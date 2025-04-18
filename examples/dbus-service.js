// SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
// SPDX-FileCopyrightText: 2020 Andy Holmes <andrew.g.r.holmes@gmail.com>

import GLib from 'gi://GLib';
import Gio from 'gi://Gio';

/*
 * An XML DBus Interface
 */
const ifaceXml = `
<node>
  <interface name="org.cinnamon.cjs.Test">
    <method name="SimpleMethod"/>
    <method name="ComplexMethod">
      <arg type="s" direction="in" name="input"/>
      <arg type="u" direction="out" name="length"/>
    </method>
    <signal name="TestSignal">
      <arg name="type" type="s"/>
      <arg name="value" type="b"/>
    </signal>
    <property name="ReadOnlyProperty" type="s" access="read"/>
    <property name="ReadWriteProperty" type="b" access="readwrite"/>
  </interface>
</node>`;


// An example of the service-side implementation of the above interface.
class Service {
    constructor() {
        this.dbus = Gio.DBusExportedObject.wrapJSObject(ifaceXml, this);
    }

    // Properties
    get ReadOnlyProperty() {
        return 'a string';
    }

    get ReadWriteProperty() {
        if (this._readWriteProperty === undefined)
            return false;

        return this._readWriteProperty;
    }

    set ReadWriteProperty(value) {
        if (this.ReadWriteProperty !== value) {
            this._readWriteProperty = value;

            // Emitting property changes over DBus
            this.dbus.emit_property_changed(
                'ReadWriteProperty',
                new GLib.Variant('b', value)
            );
        }
    }

    // Methods
    SimpleMethod() {
        print('SimpleMethod() invoked');
    }

    ComplexMethod(input) {
        print(`ComplexMethod() invoked with "${input}"`);

        return input.length;
    }

    // Signals
    emitTestSignal() {
        this.dbus.emit_signal(
            'TestSignal',
            new GLib.Variant('(sb)', ['string', false])
        );
    }
}


// Once you've created an instance of your service, you will want to own a name
// on the bus so clients can connect to it.
let serviceObj = new Service();
let serviceSignalId = 0;


function onBusAcquired(connection, _name) {
    // At this point you have acquired a connection to the bus, and you should
    // export your interfaces now.
    serviceObj.dbus.export(connection, '/org/cinnamon/cjs/Test');
}

function onNameAcquired(_connection, _name) {
    // Clients will typically start connecting and using your interface now.

    // Emit the TestSignal every few seconds
    serviceSignalId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 3, () => {
        serviceObj.emitTestSignal();

        return GLib.SOURCE_CONTINUE;
    });
}

function onNameLost(_connection, _name) {
    // Clients will know not to call methods on your interface now. Usually this
    // callback will only be invoked if you try to own a name on DBus that
    // already has an owner.

    // Stop emitting the test signal
    if (serviceSignalId > 0) {
        GLib.Source.remove(serviceSignalId);
        serviceSignalId = 0;
    }
}

let ownerId = Gio.bus_own_name(
    Gio.BusType.SESSION,
    'org.cinnamon.cjs.Test',
    Gio.BusNameOwnerFlags.NONE,
    onBusAcquired,
    onNameAcquired,
    onNameLost
);


// Start an event loop
let loop = GLib.MainLoop.new(null, false);
loop.run();

// Unowning names works just like disconnecting, but note that `onNameLost()`
// will not be invoked in this case.
Gio.bus_unown_name(ownerId);

if (serviceSignalId > 0) {
    GLib.Source.remove(serviceSignalId);
    serviceSignalId = 0;
}

