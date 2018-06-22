open Ava;

test("falsy", t =>
  t.falsy("falsy", false)
);

testFailing("falsy#fail", t => t.falsy("falsy", true));
