[@react.component]
let make = () => {
  let (theme, dispatch) = React.useContext(ThemeContext.context);
  let checked = theme.theme === Themes.Standard;
  <Toggle
    renderChecked={() => <Sun width="20" height="20" />}
    renderUnchecked={() => <Moon width="20" height="20" />}
    checked
    onChange={newValue =>
      if (checked !== newValue) {
        let newTheme = newValue ? Themes.Standard : Themes.Dark;
        dispatch(ThemeContext.ChangeTheme(newTheme));
      }
    }
  />;
};