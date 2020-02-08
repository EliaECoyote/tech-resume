type state = (
  AsyncTask.status(UseResumeData.resumeDataT),
  React.callback(UseResumeData.resumeDataT, unit),
);

let context = React.createContext((AsyncTask.Idle, () => ()));

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

[@react.component]
let make = (~children) => {
  let (state, saveData) = UseResumeData.hook();
  <Provider value=(state, saveData)> children </Provider>;
};