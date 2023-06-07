/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./src/**/*.{html,js}",
    "../../xml/**/*.xml",
    "../../../src/*.cpp",
    "../../../src/include/*.h",
  ],
  corePlugins: { preflight: false },
  darkMode: "class",
  theme: {
    extend: {
      colors: {
        neutral: {
          0: "#ffffff",
          50: "#f2f2f2",
          100: "#d8d8d8",
          150: "#bfbfbf",
          200: "#b0b1b1",
          250: "#a1a2a2",
          300: "#89898a",
          350: "#717172",
          400: "#616263",
          450: "#515152",
          500: "#3a3b3c",
          550: "#2e2f30",
          600: "#2e2f30",
          650: "#2e2f30",
          700: "#232324",
          750: "#1d1d1e",
          800: "#171818",
          850: "#111111",
          900: "#0c0c0c",
          950: "#060606",
          1000: "#000000",
        },
      },
    },
  },
  plugins: [],
};
