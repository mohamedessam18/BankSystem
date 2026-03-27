import { useMemo, useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import { useAuth } from "../context/AuthContext";

const navByRole = {
  client: [
    { label: "Dashboard", href: "/dashboard/client", icon: "dashboard" },
    { label: "Transactions", href: "/dashboard/client", icon: "transactions" }
  ],
  employee: [
    { label: "Dashboard", href: "/dashboard/employee", icon: "dashboard" },
    { label: "Clients", href: "/dashboard/employee", icon: "people" }
  ],
  admin: [
    { label: "Dashboard", href: "/dashboard/admin", icon: "dashboard" },
    { label: "Clients / Employees", href: "/dashboard/admin", icon: "people" },
    { label: "Security", href: "/dashboard/admin", icon: "security" }
  ]
};

export default function AppShell({ children }) {
  const { session } = useAuth();
  const [theme, setTheme] = useState("dark");
  const items = useMemo(() => navByRole[session.role] || navByRole.client, [session.role]);

  const wrapperClass =
    theme === "dark"
      ? "min-h-screen bg-ink bg-mesh text-white"
      : "min-h-screen bg-slate-100 text-slate-900";

  return (
    <div className={wrapperClass}>
      <div className="min-h-screen bg-[linear-gradient(180deg,rgba(255,255,255,0.04),transparent_26%)]">
        <div className="flex flex-col lg:flex-row">
          <Sidebar items={items} />
          <main className="flex-1 p-4 md:p-8">
            <Topbar
              name={session.user.name}
              role={session.role}
              theme={theme}
              onToggleTheme={() => setTheme((current) => (current === "dark" ? "light" : "dark"))}
            />
            <div className="mt-8">{children}</div>
          </main>
        </div>
      </div>
    </div>
  );
}
