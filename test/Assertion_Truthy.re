open Ava;

test.run("truthy", t => t.truthy(true));

test.failing().run("truthy#fail", t => t.truthy(false));
