import { Resolver, Query } from "type-graphql"
import { Resume } from "@entities/Resume"

@Resolver(Resume)
export class ResumeResolver {
  @Query(() => [Resume])
  resumeThemes() {
    return Resume.find()
  }
}
