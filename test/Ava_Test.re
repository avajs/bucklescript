open Ava;

testBefore(_ => ());

testAfter(_ => ());

testBeforeEach(_ => ());

testAfterEach(_ => ());

test("Test#pass", t =>
  t.pass("pass")
);

testSerial("TestSerial#pass", t => t.pass("serial"));

/* testOnly("TestOnly#pass", t =>
   t.pass("only")
   ); */

testSkip("TestSkip#pass", t => t.pass("skip"));

testTodo("TestTodo#pass");

testFailing("TestFailing#fail", t => t.fail("fail"));

test("Test#truthy", t =>
  t.truthy("truthy", true)
);

test("Test#falsy", t =>
  t.falsy("falsy", false)
);

test("Test#deepEqual", t =>
  t.deepEqual("deepEqual", ["foo"], ["foo"])
);
testFailing("Test#deepEqual", t =>
  t.deepEqual("deepEqual", ["foo"], ["bar"])
);
