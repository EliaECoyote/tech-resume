import { Entity, PrimaryGeneratedColumn, Column, BaseEntity } from "typeorm"
import { Field, Int, ObjectType } from "type-graphql"

@ObjectType()
@Entity("movies")
export class Movie extends BaseEntity {
  @Field(() => Int)
  @PrimaryGeneratedColumn()
  id: number

  @Field()
  @Column()
  title: string

  @Field(() => Int)
  @Column("int", { default: 60 })
  minutes: number
}
