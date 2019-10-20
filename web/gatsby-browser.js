/**
 * Implement Gatsby's Browser APIs in this file.
 *
 * See: https://www.gatsbyjs.org/docs/browser-apis/
 */

const React = require('react');
const ThemeContext = require('./src/context/ThemeContext').make;

exports.wrapPageElement = ({ element }) => {
  return <ThemeContext>{element}</ThemeContext>
}