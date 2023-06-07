// theme switcher Element
// const switchToggle = document.getElementById("themeToggler");

// Icons
const darkIcon = `<svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
<path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M20.354 15.354A9 9 0 018.646 3.646 9.003 9.003 0 0012 21a9.003 9.003 0 008.354-5.646z" />
</svg>`;

const lightIcon = `<svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
<path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z" />
</svg>`;

// Theme Vars
const userTheme = localStorage.getItem("theme");
const systemTheme = window.matchMedia("(prefers-color-scheme: dark)").matches;
console.log(userTheme, systemTheme);
// Initial Theme Check
const themeCheck = () => {
  if (userTheme === "dark" || (!userTheme && systemTheme)) {
    setDarkTheme();
    return;
  }
  setLightTheme();
};

// set dark theme
const setDarkTheme = () => {
  document.documentElement.classList.add("dark");
  localStorage.setItem("theme", "dark");
  const switchToggle = document.getElementById("themeToggler");
  switchToggle.classList.remove("bg-yellow-500", "-translate-x-2");
  switchToggle.classList.add("bg-gray-700", "translate-x-full");
  setTimeout(() => {
    switchToggle.innerHTML = darkIcon;
  }, 250);
};

// set light theme
const setLightTheme = () => {
  document.documentElement.classList.remove("dark");
  localStorage.setItem("theme", "light");
  const switchToggle = document.getElementById("themeToggler");
  switchToggle.classList.add("bg-yellow-500", "-translate-x-2");
  switchToggle.classList.remove("bg-gray-700", "translate-x-full");
  setTimeout(() => {
    switchToggle.innerHTML = lightIcon;
  }, 250);
};

// Manual Theme Switch
const toggleTheme = () => {
  if (document.documentElement.classList.contains("dark")) {
    document.documentElement.classList.remove("dark");
    localStorage.setItem("theme", "light");
    setLightTheme();
    return;
  }
  document.documentElement.classList.add("dark");
  localStorage.setItem("theme", "dark");
  setDarkTheme();
};

// call theme check on load
// themeCheck();

// HTML Template
{
  /* <div class="absolute top-0 bottom-0">
<button
    class="w-10 h-5 m-4 rounded-full bg-white flex items-center transition duration-300 focus:outline-none shadow"
    onclick="toggleTheme()">
    <div id="themeToggler"
        class="w-6 h-6 relative rounded-full transition duration-500 transform bg-yellow-500 -translate-x-2 p-1 text-white">
    </div>
</button>
</div> */
}
