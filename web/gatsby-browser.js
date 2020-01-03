/**
 * Implement Gatsby's Browser APIs in this file.
 *
 * See: https://www.gatsbyjs.org/docs/browser-apis/
 */

import React from 'react'
import App from './src/App'

export const wrapPageElement = ({ element }) => {
  return <App>{element}</App>
}