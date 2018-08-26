open Sync;

let hook = _ => ();

test("Sync.test", t =>
  t.pass()
);
testFailing("Sync.testFailing", t => t.fail());
before(hook);
beforeEach(hook);
after(hook);
afterAlways(hook);
afterEach(hook);
afterEachAlways(hook);
todo("Sync.todo");
