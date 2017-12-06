// Copyright 2011 Jasper St. Pierre
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

const Gi = imports._gi;
const CjsPrivate = imports.gi.CjsPrivate;
const Lang = imports.lang;
const Legacy = imports._legacy;

let GObject;

// Some common functions between GObject.Class and GObject.Interface

function _createSignals(gtype, signals) {
    for (let signalName in signals) {
        let obj = signals[signalName];
        let flags = (obj.flags !== undefined) ? obj.flags : GObject.SignalFlags.RUN_FIRST;
        let accumulator = (obj.accumulator !== undefined) ? obj.accumulator : GObject.AccumulatorType.NONE;
        let rtype = (obj.return_type !== undefined) ? obj.return_type : GObject.TYPE_NONE;
        let paramtypes = (obj.param_types !== undefined) ? obj.param_types : [];

        try {
            obj.signal_id = Gi.signal_new(gtype, signalName, flags, accumulator, rtype, paramtypes);
        } catch (e) {
            throw new TypeError('Invalid signal ' + signalName + ': ' + e.message);
        }
    }
}

function _createGTypeName(params) {
    if (params.GTypeName)
        return params.GTypeName;
    else
        return 'Gjs_' + params.Name;
}

function _getGObjectInterfaces(interfaces) {
    return interfaces.filter((iface) => iface.hasOwnProperty('$gtype'));
}

function _propertiesAsArray(params) {
    let propertiesArray = [];
    if (params.Properties) {
        for (let prop in params.Properties) {
            propertiesArray.push(params.Properties[prop]);
        }
    }
    return propertiesArray;
}

const GObjectMeta = new Lang.Class({
    Name: 'GObjectClass',
    Extends: Lang.Class,

    _init: function (params) {
        // retrieve signals and remove them from params before chaining
	let signals = params.Signals;
        delete params.Signals;

        this.parent(params);

        if (signals)
            _createSignals(this.$gtype, signals);

	let propertyObj = { };
	Object.getOwnPropertyNames(params).forEach(function(name) {
            if (name == 'Name' || name == 'Extends' || name == 'Abstract')
		return;

            let descriptor = Object.getOwnPropertyDescriptor(params, name);

            if (typeof descriptor.value === 'function') {
		let wrapped = this.prototype[name];

                if (name.slice(0, 6) == 'vfunc_') {
                    Gi.hook_up_vfunc(this.prototype, name.slice(6), wrapped);
                } else if (name.slice(0, 3) == 'on_') {
                    let id = GObject.signal_lookup(name.slice(3).replace('_', '-'), this.$gtype);
                    if (id != 0) {
                        GObject.signal_override_class_closure(id, this.$gtype, function() {
                            let argArray = Array.prototype.slice.call(arguments);
                            let emitter = argArray.shift();

                            return wrapped.apply(emitter, argArray);
                        });
                    }
                }
	    }
	}.bind(this));
    },

    _isValidClass: function(klass) {
        let proto = klass.prototype;

        if (!proto)
            return false;

        // If proto == GObject.Object.prototype, then
        // proto.__proto__ is Object, so "proto instanceof GObject.Object"
        // will return false.
        return proto == GObject.Object.prototype ||
            proto instanceof GObject.Object;
    },

    // If we want an object with a custom JSClass, we can't just
    // use a function. We have to use a custom constructor here.
    _construct: function(params) {
        if (!params.Name)
            throw new TypeError("Classes require an explicit 'Name' parameter.");
        let name = params.Name;

        let gtypename = _createGTypeName(params);

        if (!params.Extends)
            params.Extends = GObject.Object;
        let parent = params.Extends;

        if (!this._isValidClass(parent))
            throw new TypeError('GObject.Class used with invalid base class (is ' + parent + ')');

        let interfaces = params.Implements || [];
        if (parent instanceof Lang.Class)
            interfaces = interfaces.filter((iface) => !parent.implements(iface));
        let gobjectInterfaces = _getGObjectInterfaces(interfaces);

        let propertiesArray = _propertiesAsArray(params);
        delete params.Properties;

        let newClass = Gi.register_type(parent.prototype, gtypename,
            gobjectInterfaces, propertiesArray);

        // See Class.prototype._construct in _legacy.js for the reasoning
        // behind this direct prototype set.
        Object.setPrototypeOf(newClass, this.constructor.prototype);
        newClass.__super__ = parent;

        newClass._init.apply(newClass, arguments);

        Object.defineProperties(newClass.prototype, {
            '__metaclass__': { writable: false,
                               configurable: false,
                               enumerable: false,
                               value: this.constructor },
            '__interfaces__': { writable: false,
                                configurable: false,
                                enumerable: false,
                                value: interfaces }
        });

        interfaces.forEach((iface) => {
            if (iface instanceof Lang.Interface)
                iface._check(newClass.prototype);
        });

        return newClass;
    },

    // Overrides Lang.Class.implements()
    implements: function (iface) {
        if (iface instanceof GObject.Interface) {
            return GObject.type_is_a(this.$gtype, iface.$gtype);
        } else {
            return this.parent(iface);
        }
    }
});

function GObjectInterface(params) {
    return this._construct.apply(this, arguments);
}

GObjectMeta.MetaInterface = GObjectInterface;

GObjectInterface.__super__ = Lang.Interface;
GObjectInterface.prototype = Object.create(Lang.Interface.prototype);
GObjectInterface.prototype.constructor = GObjectInterface;
GObjectInterface.prototype.__name__ = 'GObjectInterface';

GObjectInterface.prototype._construct = function (params) {
    if (!params.Name) {
        throw new TypeError("Interfaces require an explicit 'Name' parameter.");
    }

    let gtypename = _createGTypeName(params);
    delete params.GTypeName;

    let interfaces = params.Requires || [];
    let gobjectInterfaces = _getGObjectInterfaces(interfaces);

    let properties = _propertiesAsArray(params);
    delete params.Properties;

    let newInterface = Gi.register_interface(gtypename, gobjectInterfaces,
        properties);

    // See Class.prototype._construct in _legacy.js for the reasoning
    // behind this direct prototype set.
    Object.setPrototypeOf(newInterface, this.constructor.prototype);
    newInterface.__super__ = GObjectInterface;
    newInterface.prototype.constructor = newInterface;

    newInterface._init.apply(newInterface, arguments);

    Object.defineProperty(newInterface.prototype, '__metaclass__', {
        writable: false,
        configurable: false,
        enumerable: false,
        value: this.constructor
    });

    return newInterface;
};

GObjectInterface.prototype._init = function (params) {
    let signals = params.Signals;
    delete params.Signals;

    Lang.Interface.prototype._init.call(this, params);

    _createSignals(this.$gtype, signals);
};

function _init() {

    GObject = this;

    function _makeDummyClass(obj, name, upperName, gtypeName, actual) {
        let gtype = GObject.type_from_name(gtypeName);
        obj['TYPE_' + upperName] = gtype;
        obj[name] = function(v) { return new actual(v); };
        obj[name].$gtype = gtype;
    }

    _makeDummyClass(this, 'VoidType', 'NONE', 'void', function() {});
    _makeDummyClass(this, 'Char', 'CHAR', 'gchar', Number);
    _makeDummyClass(this, 'UChar', 'UCHAR', 'guchar', Number);
    _makeDummyClass(this, 'Unichar', 'UNICHAR', 'gint', String);

    this.TYPE_BOOLEAN = GObject.type_from_name('gboolean');
    this.Boolean = Boolean;
    Boolean.$gtype = this.TYPE_BOOLEAN;

    _makeDummyClass(this, 'Int', 'INT', 'gint', Number);
    _makeDummyClass(this, 'UInt', 'UINT', 'guint', Number);
    _makeDummyClass(this, 'Long', 'LONG', 'glong', Number);
    _makeDummyClass(this, 'ULong', 'ULONG', 'gulong', Number);
    _makeDummyClass(this, 'Int64', 'INT64', 'gint64', Number);
    _makeDummyClass(this, 'UInt64', 'UINT64', 'guint64', Number);

    this.TYPE_ENUM = GObject.type_from_name('GEnum');
    this.TYPE_FLAGS = GObject.type_from_name('GFlags');

    _makeDummyClass(this, 'Float', 'FLOAT', 'gfloat', Number);
    this.TYPE_DOUBLE = GObject.type_from_name('gdouble');
    this.Double = Number;
    Number.$gtype = this.TYPE_DOUBLE;

    this.TYPE_STRING = GObject.type_from_name('gchararray');
    this.String = String;
    String.$gtype = this.TYPE_STRING;

    this.TYPE_POINTER = GObject.type_from_name('gpointer');
    this.TYPE_BOXED = GObject.type_from_name('GBoxed');
    this.TYPE_PARAM = GObject.type_from_name('GParam');
    this.TYPE_INTERFACE = GObject.type_from_name('GInterface');
    this.TYPE_OBJECT = GObject.type_from_name('GObject');
    this.TYPE_VARIANT = GObject.type_from_name('GVariant');

    _makeDummyClass(this, 'Type', 'GTYPE', 'GType', GObject.type_from_name);

    this.ParamSpec.char = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_char(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.uchar = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_uchar(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.int = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_int(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.uint = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_uint(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.long = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_long(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.ulong = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_ulong(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.int64 = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_int64(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.uint64 = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_uint64(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.float = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_float(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.boolean = function(name, nick, blurb, flags, default_value) {
        return GObject.param_spec_boolean(name, nick, blurb, default_value, flags);
    };

    this.ParamSpec.flags = function(name, nick, blurb, flags, flags_type, default_value) {
        return GObject.param_spec_flags(name, nick, blurb, flags_type, default_value, flags);
    };

    this.ParamSpec.enum = function(name, nick, blurb, flags, enum_type, default_value) {
        return GObject.param_spec_enum(name, nick, blurb, enum_type, default_value, flags);
    };

    this.ParamSpec.double = function(name, nick, blurb, flags, minimum, maximum, default_value) {
        return GObject.param_spec_double(name, nick, blurb, minimum, maximum, default_value, flags);
    };

    this.ParamSpec.string = function(name, nick, blurb, flags, default_value) {
        return GObject.param_spec_string(name, nick, blurb, default_value, flags);
    };

    this.ParamSpec.boxed = function(name, nick, blurb, flags, boxed_type) {
        return GObject.param_spec_boxed(name, nick, blurb, boxed_type, flags);
    };

    this.ParamSpec.object = function(name, nick, blurb, flags, object_type) {
        return GObject.param_spec_object(name, nick, blurb, object_type, flags);
    };

    this.ParamSpec.param = function(name, nick, blurb, flags, param_type) {
        return GObject.param_spec_param(name, nick, blurb, param_type, flags);
    };

    this.ParamSpec.override = Gi.override_property;

    Object.defineProperties(this.ParamSpec.prototype, {
        'name': { configurable: false,
                  enumerable: false,
                  get: function() { return this.get_name() } },
        '_nick': { configurable: false,
                   enumerable: false,
                   get: function() { return this.get_nick() } },
        'nick': { configurable: false,
                  enumerable: false,
                  get: function() { return this.get_nick() } },
        '_blurb': { configurable: false,
                    enumerable: false,
                    get: function() { return this.get_blurb() } },
        'blurb': { configurable: false,
                   enumerable: false,
                   get: function() { return this.get_blurb() } },
        'default_value': { configurable: false,
                           enumerable: false,
                           get: function() { return this.get_default_value() } },
        'flags':  { configurable: false,
                    enumerable: false,
                    get: function() { return CjsPrivate.param_spec_get_flags(this) } },
        'value_type':  { configurable: false,
                         enumerable: false,
                         get: function() { return CjsPrivate.param_spec_get_value_type(this) } },
        'owner_type':  { configurable: false,
                         enumerable: false,
                         get: function() { return CjsPrivate.param_spec_get_owner_type(this) } },
    });


    this.Class = GObjectMeta;
    this.Interface = GObjectInterface;
    this.Object.prototype.__metaclass__ = this.Class;

    // For compatibility with Lang.Class... we need a _construct
    // or the Lang.Class constructor will fail.
    this.Object.prototype._construct = function() {
        this._init.apply(this, arguments);
        return this;
    };

    // fake enum for signal accumulators, keep in sync with gi/object.c
    this.AccumulatorType = {
        NONE: 0,
        FIRST_WINS: 1,
        TRUE_HANDLED: 2
    };

    this.Object.prototype.disconnect = function(id) {
        return GObject.signal_handler_disconnect(this, id);
    };

    this.Object.prototype.handler_block = function(id) {
        return GObject.signal_handler_block(this, id);
    };

    this.Object.prototype.handler_unblock = function(id) {
        return GObject.signal_handler_unblock(this, id);
    };
}


