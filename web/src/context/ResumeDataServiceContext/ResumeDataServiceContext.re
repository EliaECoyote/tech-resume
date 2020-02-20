let context = React.createContext(ServiceResumeData.make());

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};

[@react.component]
let make = (~children) => {
  let resumeDataService = UseLazyRef.hook(ServiceResumeData.make);
  <Provider value=resumeDataService> children </Provider>;
};