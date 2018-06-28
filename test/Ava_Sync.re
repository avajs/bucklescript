open Ava.Sync;

let hook = _ => ();

test("Sync.test", t =>
  t.pass()
);
after(hook);
after_always(hook);
after_each(hook);
after_each_always(hook);
before(hook);
before_each(hook);
todo("Sync.todo");
