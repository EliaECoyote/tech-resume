type detail = {
  id: int,
  theme: Themes.t,
  description: string,
};

let themesDetails = [
  {id: 0, theme: Themes.Standard, description: "standard"},
  {id: 1, theme: Themes.Dark, description: "dark"},
  {id: 2, theme: Themes.Black, description: "black"},
];

[@react.component]
let make = () => {
  let (state, dispatch) = React.useContext(ConfigContext.context);
  let dropdownItems =
    themesDetails
    |> List.map(details =>
         <Dropdown.Item
           value={string_of_int(details.id)} key={string_of_int(details.id)}>
           {ReasonReact.string(details.description)}
         </Dropdown.Item>
       )
    |> Array.of_list
    |> React.array;

  let onChange = (event: ReactEvent.Form.t) => {
    let value = ReactEvent.Form.target(event)##value->int_of_string;
    let details = themesDetails |> List.find(details => details.id === value);
    dispatch(ConfigContext.ChangeTheme(details.theme));
  };

  let value =
    themesDetails
    |> List.find(details => details.theme === state.theme)
    |> (theme => theme.id)
    |> string_of_int;
  <Dropdown value onChange name="theme"> dropdownItems </Dropdown>;
};