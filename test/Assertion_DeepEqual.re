open Ava;

type recordExample = {
  int,
  string,
};

test.run("deepEqual", t => {
  t.deepEqual(true, true);
  t.deepEqual(3, 3);
  t.deepEqual("foo", "foo");
  t.deepEqual(["foo"], ["foo"]);
  t.deepEqual((3, "foo"), (3, "foo"));
  t.deepEqual({int: 3, string: "foo"}, {int: 3, string: "foo"});
});
test.failing().run("deepEqual#fail#bool", t => t.deepEqual(true, false));
test.failing().run("deepEqual#fail#int", t => t.deepEqual(3, 4));
test.failing().run("deepEqual#fail#string", t => t.deepEqual("foo", "bar"));
test.failing().run("deepEqual#fail#list", t =>
  t.deepEqual(["foo"], ["bar"])
);
test.failing().run("deepEqual#fail#tuple", t =>
  t.deepEqual((3, "foo"), (4, "bar"))
);
test.failing().run("deepEqual#fail#record", t =>
  t.deepEqual({int: 3, string: "foo"}, {int: 4, string: "bar"})
);
