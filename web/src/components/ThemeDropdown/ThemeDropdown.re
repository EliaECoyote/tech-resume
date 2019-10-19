type detail = {
  id: int,
  theme: Themes.t,
  description: string,
};

let themesDetails = [|
  {id: 0, theme: Themes.Standard, description: "standard"},
  {id: 1, theme: Themes.Dark, description: "dark"},
  {id: 2, theme: Themes.Black, description: "black"},
|];

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(ConfigContext.context);
  let dropdownItems =
    themesDetails
    |> Array.map(details =>
         <Dropdown.Item
           value={string_of_int(details.id)} key={string_of_int(details.id)}>
           {ReasonReact.string(details.description)}
         </Dropdown.Item>
       )
    |> React.array;

  let onChange = (event: ReactEvent.Form.t) => {
    let value = ReactEvent.Form.target(event)##value->int_of_string;
    let details =
      themesDetails->Belt.Array.getBy(details => details.id === value);
    switch (details) {
    | Some(value) => dispatch(ConfigContext.ChangeTheme(value.theme))
    | None => ()
    };
  };

  <Dropdown onChange> dropdownItems </Dropdown>;
} /* }*/ /*   <Dropdown onChange> dropdownItems </Dropdown>*/;

// type detail = {
//   id: int,
//   theme: Themes.t,
//   description: string,
// };

// let themesDetails = [|
//   {id: 1, theme: Themes.Black, description: "black"},
//   {id: 2, theme: Themes.Dark, description: "dark"},
//   {id: 3, theme: Themes.Standard, description: "standard"},
// |];

// [@react.component]
// let make = () => {
//   let (state, dispatch) = React.useContext(ConfigContext.context);
//   let dropdownItems =
//     themesDetails
//     |> Array.map(details =>
//          <Dropdown.Item
//            value={string_of_int(details.id)}
//            key={string_of_int(details.id)}>
//            {ReasonReact.string(details.description)}
//          </Dropdown.Item>
//        )
//     |> React.array;

//   let onChange = (event: ReactEvent.Form.t) => {
//     themesDetails
//     -> Belt.Array.getBy(details => details.id == 2)
//     -> Belt.Option.map(details => dispatch(ConfigContext.ChangeTheme(details.theme)));
//     ();
//   }
//     // -> Belt.Array.getBy(details => details.id === event.target.value)