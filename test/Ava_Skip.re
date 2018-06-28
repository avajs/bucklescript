open Ava.Sync;

test_skip("Sync.test_skip", t => t.pass());
test_failing_skip("Sync.test_failing_skip", t => t.fail());
Serial.test_skip("Sync.Serial.test_skip", t => t.pass());
/* Serial.test_failing_skip("Sync.Serial.test_failing_skip", t => t.fail()); */
