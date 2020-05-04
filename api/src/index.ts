import "reflect-metadata"
import { createConnection } from "typeorm"
import express from "express"
import { ApolloServer } from "apollo-server-express"
import { buildSchema } from "type-graphql"
import { HelloWorldResolver } from "./resolvers/HelloWorldResolver"
import { MovieResolver } from "./resolvers/MovieResolver"

async function startServer() {
  const app = express()

  try {
    await createConnection()

    const apolloServer = new ApolloServer({
      schema: await buildSchema({
        resolvers: [HelloWorldResolver, MovieResolver],
      }),
      context: ({ req, res }) => ({ req, res }),
    })

    apolloServer.applyMiddleware({ app, cors: false })

    app.listen(process.env.PORT, () => {
      console.log("express server started")
    })
  } catch (error) {
    console.error(error)
    process.exit(1)
  }
}

startServer()
