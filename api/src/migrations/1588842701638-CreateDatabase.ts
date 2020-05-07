import { MigrationInterface, QueryRunner } from "typeorm"

export class CreateDatabase1588842701638 implements MigrationInterface {
  name = "CreateDatabase1588842701638"

  public async up(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.query(
      `CREATE TABLE "movies" ("id" SERIAL NOT NULL, "title" character varying NOT NULL, "minutes" integer NOT NULL DEFAULT 60, CONSTRAINT "PK_c5b2c134e871bfd1c2fe7cc3705" PRIMARY KEY ("id"))`,
      undefined,
    )
    await queryRunner.query(
      `CREATE TABLE "resume_themes" ("id" uuid NOT NULL DEFAULT uuid_generate_v4(), "name" character varying NOT NULL, "createdDate" TIMESTAMP NOT NULL DEFAULT now(), "updatedDate" TIMESTAMP NOT NULL DEFAULT now(), CONSTRAINT "PK_b63291b2faeeb308d6b43a02ea8" PRIMARY KEY ("id"))`,
      undefined,
    )
    await queryRunner.query(
      `CREATE TABLE "resumes" ("id" uuid NOT NULL DEFAULT uuid_generate_v4(), "name" character varying NOT NULL, "mdTemplate" character varying NOT NULL, "createdDate" TIMESTAMP NOT NULL DEFAULT now(), "updatedDate" TIMESTAMP NOT NULL DEFAULT now(), "resumeThemeId" uuid, CONSTRAINT "PK_9c8677802096d6baece48429d2e" PRIMARY KEY ("id"))`,
      undefined,
    )
    await queryRunner.query(
      `CREATE TABLE "users" ("id" SERIAL NOT NULL, "firstName" character varying NOT NULL, "lastName" character varying NOT NULL, "age" integer NOT NULL, CONSTRAINT "PK_a3ffb1c0c8416b9fc6f907b7433" PRIMARY KEY ("id"))`,
      undefined,
    )
    await queryRunner.query(
      `ALTER TABLE "resumes" ADD CONSTRAINT "FK_00f3124beaa092e2ef641f85992" FOREIGN KEY ("resumeThemeId") REFERENCES "resume_themes"("id") ON DELETE NO ACTION ON UPDATE NO ACTION`,
      undefined,
    )
  }

  public async down(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.query(
      `ALTER TABLE "resumes" DROP CONSTRAINT "FK_00f3124beaa092e2ef641f85992"`,
      undefined,
    )
    await queryRunner.query(`DROP TABLE "users"`, undefined)
    await queryRunner.query(`DROP TABLE "resumes"`, undefined)
    await queryRunner.query(`DROP TABLE "resume_themes"`, undefined)
    await queryRunner.query(`DROP TABLE "movies"`, undefined)
  }
}
