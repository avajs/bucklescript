open Ava;

Sync.test_skip("Sync.test_skip", t => t.pass());
Sync.test_failing_skip("Sync.test_failing_skip", t => t.fail());
Sync.Serial.test_skip("Sync.Serial.test_skip", t => t.pass());
/* Sync.Serial.test_failing_skip("Sync.Serial.test_failing_skip", t => t.fail()); */

Async.test_skip("Async.test_skip", t => t.pass());
Async.test_failing_skip("Async.test_failing_skip", t => {
  let error: Js.Exn.t = [%raw {| new Error("error out") |}];
  t.cb(~error, ());
});
/* Async.Serial.test_skip("Async.Serial.test_skip", t => t.pass()); */
/* Async.Serial.test_failing_skip("Async.Serial.test_failing_skip", t => {
     let error: Js.Exn.t = [%raw {| new Error("error out") |}];
     t.cb(~error, ());
   }); */
