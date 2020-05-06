import {MigrationInterface, QueryRunner} from "typeorm";

export class CreateDatabase1588806665235 implements MigrationInterface {
    name = 'CreateDatabase1588806665235'

    public async up(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`CREATE TABLE "movies" ("id" SERIAL NOT NULL, "title" character varying NOT NULL, "minutes" integer NOT NULL DEFAULT 60, CONSTRAINT "PK_c5b2c134e871bfd1c2fe7cc3705" PRIMARY KEY ("id"))`, undefined);
        await queryRunner.query(`CREATE TABLE "resume_themes" ("id" uuid NOT NULL DEFAULT uuid_generate_v4(), "name" character varying NOT NULL, CONSTRAINT "PK_b63291b2faeeb308d6b43a02ea8" PRIMARY KEY ("id"))`, undefined);
        await queryRunner.query(`CREATE TABLE "users" ("id" SERIAL NOT NULL, "firstName" character varying NOT NULL, "lastName" character varying NOT NULL, "age" integer NOT NULL, CONSTRAINT "PK_a3ffb1c0c8416b9fc6f907b7433" PRIMARY KEY ("id"))`, undefined);
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`DROP TABLE "users"`, undefined);
        await queryRunner.query(`DROP TABLE "resume_themes"`, undefined);
        await queryRunner.query(`DROP TABLE "movies"`, undefined);
    }

}
