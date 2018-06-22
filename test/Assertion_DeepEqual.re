open Ava;

type recordExample = {
  int,
  string,
};

test("deepEqual", t => {
  t.deepEqual("deepEqual#bool", true, true);
  t.deepEqual("deepEqual#int", 3, 3);
  t.deepEqual("deepEqual#string", "foo", "foo");
  t.deepEqual("deepEqual#list", ["foo"], ["foo"]);
  t.deepEqual("deepEqual#tuple", (3, "foo"), (3, "foo"));
  t.deepEqual(
    "deepEqual#record",
    {int: 3, string: "foo"},
    {int: 3, string: "foo"},
  );
});
testFailing("deepEqual#fail#bool", t =>
  t.deepEqual("deepEqual#bool", true, false)
);
testFailing("deepEqual#fail#int", t => t.deepEqual("deepEqual#int", 3, 4));
testFailing("deepEqual#fail#string", t =>
  t.deepEqual("deepEqual#string", "foo", "bar")
);
testFailing("deepEqual#fail#list", t =>
  t.deepEqual("deepEqual#list", ["foo"], ["bar"])
);
testFailing("deepEqual#fail#tuple", t =>
  t.deepEqual("deepEqual#tuple", (3, "foo"), (4, "bar"))
);
testFailing("deepEqual#fail#record", t =>
  t.deepEqual(
    "deepEqual#record",
    {int: 3, string: "foo"},
    {int: 4, string: "bar"},
  )
);
