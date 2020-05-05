import "reflect-metadata"
import { createConnection, getConnectionOptions } from "typeorm"
import express from "express"
import { ApolloServer } from "apollo-server-express"
import { buildSchema } from "type-graphql"
import { HelloWorldResolver } from "./resolvers/HelloWorldResolver"
import { MovieResolver } from "./resolvers/MovieResolver"
import { PostgresConnectionOptions } from "typeorm/driver/postgres/PostgresConnectionOptions"
import { LoggerOptions } from "typeorm/logger/LoggerOptions"

startServer()

async function startServer() {
  const app = express()

  let retries = 0
  while (true) {
    try {
      const connectionOptions = await makeTypeOrmConfiguration()
      await createConnection(connectionOptions)
      break
    } catch (error) {
      console.error(error)
      retries += 1
      if (retries === 3) process.exit(1)
      console.log(`Retrying in 5 seconds... Retry n. ${retries}`)
      await new Promise((resolve) => setTimeout(resolve, 5000))
    }
  }

  const apolloServer = new ApolloServer({
    schema: await buildSchema({
      resolvers: [HelloWorldResolver, MovieResolver],
    }),
    context: ({ req, res }) => ({ req, res }),
  })

  apolloServer.applyMiddleware({ app, cors: false })

  // const resumeThemesRepository = getRepository(ResumeTheme)
  // for (const template of resumeThemesTemplate) {
  //   resumeThemesRepository.save(template)
  // }

  app.listen(process.env.PORT, () => {
    console.log("express server started")
  })
}

async function makeTypeOrmConfiguration() {
  const connectionOptions = await getConnectionOptions()
  const enhancedConnectionOptions: PostgresConnectionOptions = {
    ...connectionOptions,
    type: "postgres",
    database: "postgres",
    migrationsRun: true,
    host: process.env.TYPEORM_HOST,
    port: Number(process.env.TYPEORM_PORT),
    username: process.env.TYPEORM_USERNAME,
    password: process.env.TYPEORM_PASSWORD,
    logging: getLoggingConfig(),
    entities: [`${__dirname}/entities/**/*`],
    migrations: [`${__dirname}/migrations/**/*`],
    subscribers: [`${__dirname}/subscribers/**/*`],
  }
  return enhancedConnectionOptions
}

function getLoggingConfig(): LoggerOptions {
  const logging = process.env.TYPEORM_LOGGING
  if (logging == null) return []
  if (logging === "true" || logging === "TRUE" || logging === "1") return true
  if (logging === "all") return "all"
  const loggingArray = logging.split(",").map((str) => str.trim())
  const loggingArrayRegex = /^(query|schema|error|warn|info|log|migration)$/g
  if (loggingArray.every((str) => str.match(loggingArrayRegex)))
    return loggingArray as LoggerOptions
  console.error("Invalid logging configuration")
  process.exit(1)
}
