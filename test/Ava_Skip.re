Sync.testSkip("Sync.testSkip", t => t.pass());
Sync.testFailingSkip("Sync.testFailingSkip", t => t.fail());
Sync.Serial.testSkip("Sync.Serial.testSkip", t => t.pass());
/* Sync.Serial.testFailingSkip("Sync.Serial.testFailingSkip", t => t.fail()); */

Async.testSkip("Async.testSkip", t => t.pass());
Async.testFailingSkip("Async.testFailingSkip", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});
Async.Serial.testSkip("Async.Serial.testSkip", t => t.end_());
Async.Serial.testFailingSkip("Async.Serial.testFailingSkip", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});

Promise.testSkip("Promise.testSkip", t =>
  t.notThrowsAsync(() => Js.Promise.resolve())
);
Promise.testFailingSkip("Promise.testFailingSkip", t =>
  t.notThrowsAsync(() =>
    Js.Promise.make((~resolve as _, ~reject as _) =>
      Js.Exn.raiseError("Oh no")
    )
  )
);
Promise.Serial.testSkip("Promise.Serial.testSkip", t =>
  t.notThrowsAsync(() => Js.Promise.resolve())
);
/* Promise.Serial.testFailingSkip("Promise.Serial.testFailingSkip", t =>
  t.notThrowsAsync(() =>
    Js.Promise.make((~resolve as _, ~reject as _) =>
      Js.Exn.raiseError("Oh no")
    )
  )
); */
