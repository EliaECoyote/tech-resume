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

@Entity("resumes")
export class Resume extends BaseEntity {
  @PrimaryGeneratedColumn("uuid")
  id: string

  @Column()
  name: string

  @Column()
  mdTemplate: string

  @ManyToOne(() => ResumeTheme)
  resumeTheme: ResumeTheme

  @CreateDateColumn()
  createdDate: Date

  @UpdateDateColumn()
  updatedDate: Date
}
