import { Resume } from "@entities/Resume"
import { Arg, Field, InputType, Mutation, Query, Resolver } from "type-graphql"

@InputType()
class ResumeInput {
  @Field()
  mdTemplate: string

  @Field()
  name: string
}

@Resolver(Resume)
export class ResumeResolver {
  @Mutation(() => Resume)
  async addResume(@Arg("options", () => ResumeInput) options: ResumeInput) {
    const resume = await Resume.create(options)
    return resume
  }

  @Query(() => [Resume])
  resumeThemes() {
    return Resume.find()
  }
}
