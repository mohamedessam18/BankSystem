import { LayoutDashboard, LogOut, ShieldCheck, Users, WalletCards } from "lucide-react";
import { NavLink } from "react-router-dom";
import { useAuth } from "../context/AuthContext";

const iconMap = {
  dashboard: LayoutDashboard,
  people: Users,
  transactions: WalletCards,
  security: ShieldCheck
};

export default function Sidebar({ items }) {
  const { logout } = useAuth();

  return (
    <aside className="w-full max-w-72 border-r border-white/10 bg-slate-950/40 p-6 backdrop-blur-xl">
      <div>
        <div className="mb-10">
          <p className="text-xs uppercase tracking-[0.4em] text-aqua">Fintech Suite</p>
          <h1 className="mt-3 font-display text-3xl text-white">Secure Banking</h1>
        </div>
        <nav className="space-y-2">
          {items.map((item) => {
            const Icon = iconMap[item.icon];
            return (
              <NavLink
                key={item.label}
                to={item.href}
                className={({ isActive }) =>
                  `flex items-center gap-3 rounded-2xl px-4 py-3 text-sm transition ${
                    isActive ? "bg-white/[0.12] text-white shadow-glow" : "text-slate-400 hover:bg-white/[0.06] hover:text-white"
                  }`
                }
              >
                <Icon className="h-4 w-4" />
                {item.label}
              </NavLink>
            );
          })}

          <button
            onClick={logout}
            className="flex w-full items-center gap-3 rounded-2xl border border-white/10 px-4 py-3 text-sm text-slate-300 transition hover:border-red-400/30 hover:bg-red-500/10 hover:text-white"
          >
            <LogOut className="h-4 w-4" />
            Logout
          </button>
        </nav>
      </div>
    </aside>
  );
}
