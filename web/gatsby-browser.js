/**
 * Implement Gatsby's Browser APIs in this file.
 *
 * See: https://www.gatsbyjs.org/docs/browser-apis/
 */

const React = require('react');
const App = require('./src/App').make;

exports.wrapPageElement = ({ element }) => {
  return <App>{element}</App>
}