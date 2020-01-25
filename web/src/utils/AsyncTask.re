type status('data) =
  | Idle
  | Fetching
  | Error
  | Success('data);

type events('data) =
  | LoadData
  | LoadSuccess('data)
  | LoadFailed;

let reducer = (~state, ~event) =>
  switch (state, event) {
  | (_, LoadData) => UseMachine.Valid(Fetching)
  | (Fetching, LoadSuccess(value)) => UseMachine.Valid(Success(value))
  | (Fetching, LoadFailed) => UseMachine.Valid(Error)
  | _ =>
    UseMachine.Invalid(("[AsyncTask] invalid machine event", state, event))
  };