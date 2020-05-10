import { Field, ObjectType } from "type-graphql"
import {
  BaseEntity,
  Column,
  CreateDateColumn,
  Entity,
  ManyToOne,
  PrimaryGeneratedColumn,
  UpdateDateColumn,
} from "typeorm"
import { ResumeTheme } from "./ResumeTheme"

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
