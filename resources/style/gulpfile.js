/* eslint-env node */
const { series, src, dest } = require("gulp");
const sass = require("gulp-sass")(require("sass"));
const postcss = require("gulp-postcss");
const autoprefixer = require("autoprefixer");
const cssnano = require("cssnano");
const sourcemaps = require("gulp-sourcemaps");

const outputFolder = "../css/";

function compileSass() {
  return src("theme.scss")
    .pipe(sourcemaps.init())
    .pipe(sass())
    .pipe(postcss([autoprefixer, cssnano]))
    .pipe(sourcemaps.write("."))
    .pipe(dest(outputFolder));
}

// used to run compileSass when writing gulp in the terminal
exports.compileSass = compileSass;
exports.default = series(compileSass);