import { ShieldCheck, Users, Wallet } from "lucide-react";
import AppShell from "../layouts/AppShell";
import GlassCard from "../components/GlassCard";
import { Skeleton } from "../components/Loader";
import { adminApi, employeeApi } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";

export default function AdminSecurityPage() {
  const employeesState = useDashboardData(async () => {
    const response = await adminApi.getEmployees();
    return response.data.data.employees;
  }, []);

  const clientsState = useDashboardData(async () => {
    const response = await employeeApi.getClients();
    return response.data.data.clients;
  }, []);

  const employeeCount = employeesState.data?.length || 0;
  const clientCount = clientsState.data?.length || 0;
  const averageBalance = clientCount
    ? Math.round(clientsState.data.reduce((total, client) => total + Number(client.balance), 0) / clientCount)
    : 0;

  return (
    <AppShell>
      <div className="grid gap-6">
        <GlassCard>
          <p className="text-sm uppercase tracking-[0.35em] text-aqua">Security</p>
          <h3 className="mt-3 font-display text-3xl text-white">Operational security view</h3>
          <p className="mt-3 max-w-2xl text-sm leading-7 text-slate-300">
            This page gives admins a cleaner governance snapshot while keeping editing tools in the people section.
          </p>
        </GlassCard>

        {employeesState.loading || clientsState.loading ? (
          <Skeleton className="h-80" />
        ) : (
          <div className="grid gap-6 md:grid-cols-3">
            <GlassCard>
              <ShieldCheck className="h-6 w-6 text-aqua" />
              <h4 className="mt-4 font-display text-2xl text-white">Protected roles</h4>
              <p className="mt-3 text-sm leading-7 text-slate-300">Admins can manage employees, but elevated manager access stays separate from public signup.</p>
            </GlassCard>
            <GlassCard>
              <Users className="h-6 w-6 text-aqua" />
              <h4 className="mt-4 font-display text-2xl text-white">{employeeCount}</h4>
              <p className="mt-3 text-sm leading-7 text-slate-300">Employees currently under administrative supervision.</p>
            </GlassCard>
            <GlassCard>
              <Wallet className="h-6 w-6 text-aqua" />
              <h4 className="mt-4 font-display text-2xl text-white">${averageBalance.toLocaleString()}</h4>
              <p className="mt-3 text-sm leading-7 text-slate-300">Average client balance across visible client accounts.</p>
            </GlassCard>
          </div>
        )}
      </div>
    </AppShell>
  );
}
