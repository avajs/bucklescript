open Ava;

test("truthy", t =>
  t.truthy("truthy", true)
);

testFailing("truthy#fail", t =>
  t.truthy("truthy", false)
);
