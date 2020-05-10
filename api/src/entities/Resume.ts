import {
  Entity,
  PrimaryGeneratedColumn,
  Column,
  BaseEntity,
  ManyToOne,
  CreateDateColumn,
  UpdateDateColumn,
} from "typeorm"
import { ResumeTheme } from "./ResumeTheme"
import { ObjectType, Field } from "type-graphql"

@ObjectType()
@Entity("resumes")
export class Resume extends BaseEntity {
  @Field()
  @PrimaryGeneratedColumn("uuid")
  id: string

  @Field()
  @Column()
  name: string

  @Field()
  @Column()
  mdTemplate: string

  @Field(() => ResumeTheme)
  @ManyToOne(() => ResumeTheme)
  resumeTheme: ResumeTheme

  @Field()
  @CreateDateColumn()
  createdDate: Date

  @Field()
  @UpdateDateColumn()
  updatedDate: Date
}
