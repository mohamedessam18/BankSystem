import { BellDot, MoonStar, SunMedium } from "lucide-react";

export default function Topbar({ name, role, theme, onToggleTheme }) {
  return (
    <div className="flex flex-col gap-4 rounded-3xl border border-white/10 bg-white/[0.06] p-5 shadow-panel backdrop-blur-xl md:flex-row md:items-center md:justify-between">
      <div>
        <p className="text-sm text-slate-400">Welcome back</p>
        <h2 className="mt-1 font-display text-2xl text-white">{name}</h2>
      </div>
      <div className="flex items-center gap-3">
        <span className="rounded-full border border-aqua/30 bg-aqua/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.3em] text-aqua">
          {role}
        </span>
        <button
          onClick={onToggleTheme}
          className="rounded-2xl border border-white/10 bg-white/5 p-3 text-slate-200 transition hover:bg-white/10"
        >
          {theme === "dark" ? <SunMedium className="h-4 w-4" /> : <MoonStar className="h-4 w-4" />}
        </button>
        <button className="rounded-2xl border border-white/10 bg-white/5 p-3 text-slate-200 transition hover:bg-white/10">
          <BellDot className="h-4 w-4" />
        </button>
      </div>
    </div>
  );
}
