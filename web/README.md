# Web module

This module is built with [gatsby](https://www.gatsbyjs.org/), and is deployed on [firebase hosting](https://firebase.google.com/docs/hosting).\
When the docker container is running, the webapp will be served on `http://localhost:8080`.
(Graphiql webapp will be served in `http://localhost:8000/___graphql`)

## Authentication

Client side authentication is handled by [firebase auth](https://firebase.google.com/docs/auth) and [firebase ui](https://github.com/firebase/firebaseui-web) modules

## Client caching

Client caching is described in the [firebase.json](web/firebase.json) file and follows the following docs:

- [Firebase hosting headers config documentation](https://firebase.google.com/docs/hosting/full-config#headers)
- [Gatsby caching documentation](https://github.com/gatsbyjs/gatsby/blob/master/docs/docs/caching.md)
