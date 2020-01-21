/**
 * Implement Gatsby's SSR (Server Side Rendering) APIs in this file.
 *
 * See: https://www.gatsbyjs.org/docs/ssr-apis/
 */

import React from 'react';
import App from './lib/js/src/App.bs'
import { renderToString } from "react-dom/server"
import { renderStylesToString } from 'emotion-server'

export const replaceRenderer = ({ bodyComponent, replaceBodyHTMLString }) => {
  const bodyHTML = renderToString(bodyComponent)
  const resultHTML = renderStylesToString(bodyHTML)
  replaceBodyHTMLString(resultHTML)
}

export const wrapPageElement = ({ element }) => {
  return <App>{element}</App>
}