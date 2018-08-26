open Sync.Serial;

let hook = _ => ();

test("Sync.Serial.test", t =>
  t.pass()
);
testFailing("Sync.Serial.testFailing", t => t.fail());
before(hook);
beforeEach(hook);
after(hook);
afterAlways(hook);
afterEach(hook);
afterEachAlways(hook);
todo("Sync.Serial.todo");
