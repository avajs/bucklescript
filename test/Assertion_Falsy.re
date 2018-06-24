open Ava;

test.run("falsy", t => t.falsy(false));

test.failing().run("falsy#fail", t => t.falsy(true));
