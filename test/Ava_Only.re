open Ava.Sync;

test_only("Sync.test_only", t => t.pass());
test_failing_only("Sync.test_failing_only", t => t.fail());
Serial.test_only("Sync.Serial.test_only", t => t.pass());
/* Serial.test_failing_only("Sync.Serial.test_failing_only", t => t.fail()); */
