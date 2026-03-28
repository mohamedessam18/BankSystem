import { useEffect, useRef, useState } from "react";
import { BellDot, MoonStar, SunMedium } from "lucide-react";
import { useNotifications } from "../context/NotificationContext";

export default function Topbar({ name, role, theme, onToggleTheme }) {
  const [open, setOpen] = useState(false);
  const panelRef = useRef(null);
  const { items, unreadCount, loading, markAllRead } = useNotifications();

  useEffect(() => {
    const handleClickOutside = (event) => {
      if (panelRef.current && !panelRef.current.contains(event.target)) {
        setOpen(false);
      }
    };

    document.addEventListener("mousedown", handleClickOutside);
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, []);

  const openPanel = async () => {
    setOpen((current) => !current);
    if (unreadCount > 0) {
      await markAllRead();
    }
  };

  return (
    <div className="relative z-20 flex flex-col gap-4 rounded-3xl border border-white/10 bg-white/[0.06] p-5 shadow-panel backdrop-blur-xl md:flex-row md:items-center md:justify-between">
      <div>
        <p className="text-sm text-slate-400">Welcome back</p>
        <h2 className="mt-1 font-display text-2xl text-white">{name}</h2>
      </div>
      <div className="flex items-center gap-3" ref={panelRef}>
        <span className="rounded-full border border-aqua/30 bg-aqua/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.3em] text-aqua">
          {role}
        </span>
        <button
          onClick={onToggleTheme}
          className="rounded-2xl border border-white/10 bg-white/5 p-3 text-slate-200 transition hover:bg-white/10"
        >
          {theme === "dark" ? <SunMedium className="h-4 w-4" /> : <MoonStar className="h-4 w-4" />}
        </button>
        <button onClick={openPanel} className="relative rounded-2xl border border-white/10 bg-white/5 p-3 text-slate-200 transition hover:bg-white/10">
          <BellDot className="h-4 w-4" />
          {unreadCount > 0 ? (
            <span className="absolute -right-1 -top-1 flex h-5 min-w-5 items-center justify-center rounded-full bg-aqua px-1 text-[10px] font-bold text-ink">
              {unreadCount > 9 ? "9+" : unreadCount}
            </span>
          ) : null}
        </button>
        {open ? (
          <div className="fixed right-6 top-24 z-[120] w-[22rem] rounded-[1.75rem] border border-white/10 bg-slate-950/95 p-4 shadow-[0_24px_80px_rgba(0,0,0,0.55)] backdrop-blur-2xl md:right-8">
            <div className="flex items-center justify-between gap-3">
              <div>
                <p className="text-xs uppercase tracking-[0.28em] text-aqua">Notifications</p>
                <h3 className="mt-2 font-display text-xl text-white">Live activity feed</h3>
              </div>
              <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1 text-xs text-slate-300">
                {items.length}
              </span>
            </div>
            <div className="mt-4 max-h-80 space-y-3 overflow-y-auto pr-1">
              {loading ? (
                <div className="rounded-2xl border border-white/10 bg-white/5 px-4 py-5 text-sm text-slate-300">
                  Loading notifications...
                </div>
              ) : items.length === 0 ? (
                <div className="rounded-2xl border border-white/10 bg-white/5 px-4 py-5 text-sm text-slate-300">
                  No notifications yet for this role.
                </div>
              ) : (
                items.map((item) => (
                  <div key={item.id} className={`rounded-2xl border px-4 py-3 ${item.read ? "border-white/10 bg-white/[0.04]" : "border-aqua/20 bg-aqua/10"}`}>
                    <div className="flex items-start justify-between gap-3">
                      <p className="text-sm font-semibold text-white">{item.title}</p>
                      <span className="text-[11px] text-slate-400">{item.createdAt}</span>
                    </div>
                    <p className="mt-2 text-sm leading-6 text-slate-300">{item.detail}</p>
                  </div>
                ))
              )}
            </div>
          </div>
        ) : null}
      </div>
    </div>
  );
}
