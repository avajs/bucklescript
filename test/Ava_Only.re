Sync.testOnly("Sync.testOnly", t => t.pass());
Sync.testFailingOnly("Sync.testFailingOnly", t => t.fail());
Sync.Serial.testOnly("Sync.Serial.testOnly", t => t.pass());
/* Sync.Serial.testFailingOnly("Sync.Serial.testFailingOnly", t => t.fail()); */

Async.testOnly("Async.testOnly", t => t.end_());
Async.testFailingOnly("Async.testFailingOnly", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});
Async.Serial.testOnly("Async.Serial.testOnly", t => t.end_());
Async.Serial.testFailingOnly("Async.Serial.testFailingOnly", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});

Promise.testOnly("Promise.testOnly", t =>
  t.notThrowsAsync(() => Js.Promise.resolve())
);
Promise.testFailingOnly("Promise.testFailingOnly", t =>
  t.notThrowsAsync(() =>
    Js.Promise.make((~resolve as _, ~reject as _) =>
      Js.Exn.raiseError("Oh no")
    )
  )
);
Promise.Serial.testOnly("Promise.Serial.testOnly", t =>
  t.notThrowsAsync(() => Js.Promise.resolve())
);
/* Promise.Serial.testFailingOnly("Promise.Serial.testFailingOnly", t =>
     t.notThrowsAsync(() =>
       Js.Promise.make((~resolve as _, ~reject as _) =>
         Js.Exn.raiseError("Oh no")
       )
     )
   ); */
