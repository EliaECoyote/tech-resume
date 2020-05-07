import { Resolver, Query } from "type-graphql"
import { ResumeTheme } from "@entities/ResumeTheme"

@Resolver(ResumeTheme)
export class ResumeThemeResolver {
  @Query(() => [ResumeTheme])
  resumeThemes() {
    return ResumeTheme.find()
  }
}
