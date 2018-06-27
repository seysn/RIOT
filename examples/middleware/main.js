/*
 * Copyright (C) 2018 Inria
 *               2017 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

print("IoT-Lab-M3 RIOT javascript demo");
print("Test riot.echo()");
riot.echo("Coucou");
print("Test timer.now()");
var now = timer.now();
print("Now is " + now);
