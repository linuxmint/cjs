// SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
// SPDX-FileCopyrightText: 2024 Gary Li <gary.li1@uwaterloo.ca>
const doImport = async () => {
    await import('./sideEffect3.js?bar=baz');
};

await doImport();
