/** @type {import('tailwindcss').Config} */
export default {
  content: ["./index.html", "./src/**/*.{js,jsx}"],
  theme: {
    extend: {
      fontFamily: {
        display: ["'Space Grotesk'", "sans-serif"],
        body: ["'Manrope'", "sans-serif"]
      },
      colors: {
        ink: "#08111f",
        shell: "#0b1526",
        aqua: "#4df6d2",
        iris: "#7c9cff",
        sunset: "#ff7b7b"
      },
      boxShadow: {
        glow: "0 0 0 1px rgba(255,255,255,0.08), 0 24px 80px rgba(76,246,210,0.15)",
        panel: "0 20px 60px rgba(8,17,31,0.45)"
      },
      backgroundImage: {
        mesh: "radial-gradient(circle at top left, rgba(77,246,210,0.18), transparent 30%), radial-gradient(circle at top right, rgba(124,156,255,0.18), transparent 24%), radial-gradient(circle at bottom center, rgba(255,123,123,0.12), transparent 28%)"
      }
    }
  },
  plugins: []
};
