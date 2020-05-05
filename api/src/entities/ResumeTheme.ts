import { Entity, PrimaryGeneratedColumn, Column, BaseEntity } from "typeorm"

@Entity("resume_themes")
export class ResumeTheme extends BaseEntity {
  @PrimaryGeneratedColumn("uuid")
  id: string

  @Column()
  name: string
}
