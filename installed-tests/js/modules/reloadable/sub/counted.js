// SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
/* exported count */

globalThis.reloadableCount = (globalThis.reloadableCount ?? 0) + 1;
var count = globalThis.reloadableCount;
