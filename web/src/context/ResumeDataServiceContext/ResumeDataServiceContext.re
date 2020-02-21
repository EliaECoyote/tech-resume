let context = React.createContext(None);

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

[@react.component]
let make = (~children) => {
  let resumeDataService = UseLazyRef.hook(Service.ResumeData.make);
  <Provider value={Some(resumeDataService)}> children </Provider>;
};