import { useMemo, useState } from "react";
import {
  Activity,
  ArrowRightLeft,
  BriefcaseBusiness,
  Building2,
  Crown,
  Landmark,
  RefreshCcw,
  ShieldPlus,
  Sparkles,
  TrendingUp,
  UserPlus,
  Wallet
} from "lucide-react";
import AppShell from "../layouts/AppShell";
import ActionModal from "../components/ActionModal";
import DataTable from "../components/DataTable";
import FormField from "../components/FormField";
import GlassCard from "../components/GlassCard";
import { Skeleton } from "../components/Loader";
import { adminApi, clientApi, employeeApi, extractApiError, managerApi } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";
import { useToast } from "../context/ToastContext";

const blankClient = { name: "", password: "", balance: "1500" };
const blankEmployee = { name: "", password: "", salary: "5000" };
const blankAdmin = { name: "", password: "", salary: "5000" };
const blankDesk = { clientId: "", amount: "", fromClientId: "", toClientId: "", transferAmount: "" };

function formatCurrency(value) {
  return `$${Number(value || 0).toLocaleString()}`;
}

function ManagerMetricCard({ label, value, caption, icon: Icon, accent }) {
  return (
    <div className={`relative overflow-hidden rounded-[2rem] border p-6 shadow-panel ${accent}`}>
      <div className="absolute inset-x-0 top-0 h-24 bg-gradient-to-b from-white/10 to-transparent" />
      <div className="relative flex items-start justify-between gap-4">
        <div>
          <p className="text-xs uppercase tracking-[0.3em] text-slate-300/80">{label}</p>
          <h4 className="mt-4 font-display text-3xl text-white">{value}</h4>
          <p className="mt-2 text-sm text-slate-300/75">{caption}</p>
        </div>
        <div className="rounded-2xl border border-white/10 bg-white/10 p-3 text-white">
          <Icon className="h-5 w-5" />
        </div>
      </div>
    </div>
  );
}

function ManagerActionTile({ title, description, icon: Icon, onClick, accent }) {
  return (
    <button
      onClick={onClick}
      className={`group rounded-[1.75rem] border p-5 text-left transition hover:-translate-y-0.5 hover:shadow-panel ${accent}`}
    >
      <div className="flex items-center justify-between gap-4">
        <div className="rounded-2xl border border-white/10 bg-white/10 p-3 text-white">
          <Icon className="h-5 w-5" />
        </div>
        <Sparkles className="h-4 w-4 text-white/60 transition group-hover:text-white" />
      </div>
      <h5 className="mt-5 font-display text-2xl text-white">{title}</h5>
      <p className="mt-2 text-sm leading-6 text-slate-200/80">{description}</p>
    </button>
  );
}

function SectionHeading({ eyebrow, title, description, action }) {
  return (
    <div className="mb-5 flex flex-col gap-4 md:flex-row md:items-end md:justify-between">
      <div>
        <p className="text-sm uppercase tracking-[0.32em] text-aqua">{eyebrow}</p>
        <h4 className="mt-2 font-display text-3xl text-white">{title}</h4>
        {description ? <p className="mt-2 max-w-2xl text-sm leading-7 text-slate-300">{description}</p> : null}
      </div>
      {action}
    </div>
  );
}

export default function ManagerDashboardPage() {
  const { pushToast } = useToast();
  const [query, setQuery] = useState("");
  const [submitting, setSubmitting] = useState(false);
  const [deskLoading, setDeskLoading] = useState("");
  const [clientForm, setClientForm] = useState(blankClient);
  const [employeeForm, setEmployeeForm] = useState(blankEmployee);
  const [adminForm, setAdminForm] = useState(blankAdmin);
  const [deskForm, setDeskForm] = useState(blankDesk);
  const [editingClient, setEditingClient] = useState(null);
  const [editingEmployee, setEditingEmployee] = useState(null);
  const [editingAdmin, setEditingAdmin] = useState(null);
  const [openClientModal, setOpenClientModal] = useState(false);
  const [openEmployeeModal, setOpenEmployeeModal] = useState(false);
  const [openAdminModal, setOpenAdminModal] = useState(false);

  const overviewState = useDashboardData(async () => {
    const response = await managerApi.getOverview();
    return response.data.data;
  }, []);

  const clientsState = useDashboardData(async () => {
    const response = await employeeApi.getClients();
    return response.data.data.clients;
  }, []);

  const employeesState = useDashboardData(async () => {
    const response = await adminApi.getEmployees();
    return response.data.data.employees;
  }, []);

  const adminsState = useDashboardData(async () => {
    const response = await managerApi.getAdmins();
    return response.data.data.admins;
  }, []);

  const refreshOverview = async () => {
    const response = await managerApi.getOverview();
    overviewState.setData(response.data.data);
  };

  const refreshClients = async () => {
    const response = await employeeApi.getClients();
    clientsState.setData(response.data.data.clients);
  };

  const refreshEmployees = async () => {
    const response = await adminApi.getEmployees();
    employeesState.setData(response.data.data.employees);
  };

  const refreshAdmins = async () => {
    const response = await managerApi.getAdmins();
    adminsState.setData(response.data.data.admins);
  };

  const refreshAll = async () => {
    await Promise.all([refreshOverview(), refreshClients(), refreshEmployees(), refreshAdmins()]);
  };

  const searchable = (row) => `${row.name} ${row.id}`.toLowerCase().includes(query.toLowerCase());
  const filteredClients = useMemo(() => (clientsState.data || []).filter(searchable), [clientsState.data, query]);
  const filteredEmployees = useMemo(() => (employeesState.data || []).filter(searchable), [employeesState.data, query]);
  const filteredAdmins = useMemo(() => (adminsState.data || []).filter(searchable), [adminsState.data, query]);

  const closeClientModal = () => {
    setOpenClientModal(false);
    setEditingClient(null);
    setClientForm(blankClient);
  };

  const closeEmployeeModal = () => {
    setOpenEmployeeModal(false);
    setEditingEmployee(null);
    setEmployeeForm(blankEmployee);
  };

  const closeAdminModal = () => {
    setOpenAdminModal(false);
    setEditingAdmin(null);
    setAdminForm(blankAdmin);
  };

  const submitClient = async (event) => {
    event.preventDefault();
    setSubmitting(true);
    try {
      if (editingClient) {
        await employeeApi.updateClient(editingClient.id, { ...clientForm, balance: Number(clientForm.balance) });
      } else {
        await employeeApi.addClient({ ...clientForm, balance: Number(clientForm.balance) });
      }
      await Promise.all([refreshClients(), refreshOverview()]);
      closeClientModal();
      pushToast({ title: editingClient ? "Client updated successfully." : "Client created successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const submitEmployee = async (event) => {
    event.preventDefault();
    setSubmitting(true);
    try {
      if (editingEmployee) {
        await adminApi.updateEmployee(editingEmployee.id, { ...employeeForm, salary: Number(employeeForm.salary) });
      } else {
        await adminApi.addEmployee({ ...employeeForm, salary: Number(employeeForm.salary) });
      }
      await Promise.all([refreshEmployees(), refreshOverview()]);
      closeEmployeeModal();
      pushToast({ title: editingEmployee ? "Employee updated successfully." : "Employee created successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const submitAdmin = async (event) => {
    event.preventDefault();
    setSubmitting(true);
    try {
      if (editingAdmin) {
        await managerApi.updateAdmin(editingAdmin.id, { ...adminForm, salary: Number(adminForm.salary) });
      } else {
        await managerApi.addAdmin({ ...adminForm, salary: Number(adminForm.salary) });
      }
      await Promise.all([refreshAdmins(), refreshOverview()]);
      closeAdminModal();
      pushToast({ title: editingAdmin ? "Admin updated successfully." : "Admin created successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const runDeskAction = async (type) => {
    setDeskLoading(type);
    try {
      if (type === "deposit") {
        await clientApi.deposit(Number(deskForm.clientId), Number(deskForm.amount));
      } else if (type === "withdraw") {
        await clientApi.withdraw(Number(deskForm.clientId), Number(deskForm.amount));
      } else {
        await clientApi.transfer({
          fromClientId: Number(deskForm.fromClientId),
          toClientId: Number(deskForm.toClientId),
          amount: Number(deskForm.transferAmount)
        });
      }

      setDeskForm(blankDesk);
      await Promise.all([refreshClients(), refreshOverview()]);
      pushToast({ title: `Manager ${type} request completed successfully.` });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setDeskLoading("");
    }
  };

  const overview = overviewState.data;
  const metrics = overview?.metrics;
  const insights = overview?.insights;

  const summaryCards = [
    {
      label: "Clients",
      value: metrics?.clientsCount ?? 0,
      caption: "Active portfolio accounts",
      icon: Wallet,
      accent: "border-cyan-300/10 bg-gradient-to-br from-cyan-400/20 via-slate-900/90 to-slate-950"
    },
    {
      label: "Employees",
      value: metrics?.employeesCount ?? 0,
      caption: "Service team members",
      icon: BriefcaseBusiness,
      accent: "border-emerald-300/10 bg-gradient-to-br from-emerald-400/18 via-slate-900/90 to-slate-950"
    },
    {
      label: "Admins",
      value: metrics?.adminsCount ?? 0,
      caption: "Operational supervisors",
      icon: ShieldPlus,
      accent: "border-amber-300/10 bg-gradient-to-br from-amber-400/18 via-slate-900/90 to-slate-950"
    },
    {
      label: "Portfolio Funds",
      value: formatCurrency(metrics?.totalClientBalances),
      caption: "Total client balances",
      icon: Landmark,
      accent: "border-fuchsia-300/10 bg-gradient-to-br from-fuchsia-400/18 via-slate-900/90 to-slate-950"
    }
  ];

  const quickActions = [
    {
      title: "Create Admin",
      description: "Provision a new operational supervisor without exposing admin signup publicly.",
      icon: Crown,
      onClick: () => setOpenAdminModal(true),
      accent: "border-amber-300/10 bg-gradient-to-br from-amber-300/15 to-slate-950"
    },
    {
      title: "Add Employee",
      description: "Expand the service team directly from the manager workspace.",
      icon: UserPlus,
      onClick: () => setOpenEmployeeModal(true),
      accent: "border-emerald-300/10 bg-gradient-to-br from-emerald-300/15 to-slate-950"
    },
    {
      title: "Open Client Account",
      description: "Launch a new client profile with protected starting balance controls.",
      icon: Wallet,
      onClick: () => setOpenClientModal(true),
      accent: "border-cyan-300/10 bg-gradient-to-br from-cyan-300/15 to-slate-950"
    }
  ];

  const clientColumns = [
    { key: "id", label: "Client ID" },
    { key: "name", label: "Name" },
    { key: "balance", label: "Balance", render: (row) => formatCurrency(row.balance) },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button
          onClick={() => {
            setEditingClient(row);
            setClientForm({ name: row.name, password: "", balance: String(row.balance) });
            setOpenClientModal(true);
          }}
          className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10"
        >
          Edit
        </button>
      )
    }
  ];

  const employeeColumns = [
    { key: "id", label: "Employee ID" },
    { key: "name", label: "Name" },
    { key: "salary", label: "Salary", render: (row) => formatCurrency(row.salary) },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button
          onClick={() => {
            setEditingEmployee(row);
            setEmployeeForm({ name: row.name, password: "", salary: String(row.salary) });
            setOpenEmployeeModal(true);
          }}
          className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10"
        >
          Edit
        </button>
      )
    }
  ];

  const adminColumns = [
    { key: "id", label: "Admin ID" },
    { key: "name", label: "Name" },
    { key: "salary", label: "Salary", render: (row) => formatCurrency(row.salary) },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button
          onClick={() => {
            setEditingAdmin(row);
            setAdminForm({ name: row.name, password: "", salary: String(row.salary) });
            setOpenAdminModal(true);
          }}
          className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10"
        >
          Edit
        </button>
      )
    }
  ];

  return (
    <AppShell>
      <div className="grid gap-6">
        {(overviewState.error || clientsState.error || employeesState.error || adminsState.error) && (
          <GlassCard className="border-red-400/20 bg-red-500/10">
            <p className="text-sm font-semibold text-red-100">The manager workspace could not load part of the system data.</p>
            <p className="mt-2 text-sm text-red-100/80">
              {overviewState.error || clientsState.error || employeesState.error || adminsState.error}
            </p>
          </GlassCard>
        )}

        <section className="relative overflow-hidden rounded-[2.25rem] border border-white/10 bg-[radial-gradient(circle_at_top_left,rgba(77,246,210,0.18),transparent_28%),radial-gradient(circle_at_85%_20%,rgba(250,204,21,0.14),transparent_24%),linear-gradient(135deg,rgba(7,15,28,0.98),rgba(9,19,35,0.96))] p-6 shadow-panel md:p-8">
          <div className="absolute inset-y-0 right-0 hidden w-[32%] bg-[linear-gradient(180deg,rgba(255,255,255,0.06),transparent)] lg:block" />
          <div className="relative grid gap-8 xl:grid-cols-[1.2fr_0.8fr]">
            <div>
              <div className="inline-flex items-center gap-2 rounded-full border border-white/10 bg-white/5 px-4 py-2 text-xs font-semibold uppercase tracking-[0.28em] text-aqua">
                <Crown className="h-4 w-4" />
                Manager Console
              </div>
              <h1 className="mt-6 max-w-3xl font-display text-4xl leading-tight text-white md:text-5xl">
                See the whole bank at once and act without friction.
              </h1>
              <p className="mt-5 max-w-2xl text-sm leading-8 text-slate-300">
                This workspace is designed for the one internal manager account. It combines leadership oversight, people management, and recovery actions into a single clear dashboard so the manager can make decisions fast.
              </p>

              <div className="mt-8 flex flex-col gap-3 sm:flex-row">
                <button onClick={refreshAll} className="inline-flex items-center justify-center gap-3 rounded-2xl bg-aqua px-5 py-3 text-sm font-semibold text-ink transition hover:shadow-glow">
                  <RefreshCcw className="h-4 w-4" />
                  Refresh all data
                </button>
                <button onClick={() => setOpenAdminModal(true)} className="inline-flex items-center justify-center gap-3 rounded-2xl border border-white/10 bg-white/5 px-5 py-3 text-sm font-semibold text-white transition hover:bg-white/10">
                  <ShieldPlus className="h-4 w-4" />
                  Add admin
                </button>
              </div>

              <div className="mt-8 grid gap-4 md:grid-cols-3">
                {quickActions.map((action) => (
                  <ManagerActionTile key={action.title} {...action} />
                ))}
              </div>
            </div>

            <div className="grid gap-4 self-start">
              <div className="rounded-[2rem] border border-white/10 bg-white/[0.06] p-6 backdrop-blur-xl">
                <p className="text-xs uppercase tracking-[0.28em] text-slate-400">Leadership pulse</p>
                {overviewState.loading ? (
                  <Skeleton className="mt-4 h-40" />
                ) : (
                  <div className="mt-5 space-y-4">
                    <div className="rounded-3xl border border-white/10 bg-white/5 p-4">
                      <div className="flex items-center justify-between text-sm text-slate-300">
                        <span>Top client</span>
                        <TrendingUp className="h-4 w-4 text-aqua" />
                      </div>
                      <h3 className="mt-3 font-display text-2xl text-white">{insights?.topClient?.name || "N/A"}</h3>
                      <p className="mt-2 text-sm text-slate-400">{formatCurrency(insights?.topClient?.balance)} under management</p>
                    </div>
                    <div className="rounded-3xl border border-white/10 bg-white/5 p-4">
                      <div className="flex items-center justify-between text-sm text-slate-300">
                        <span>Highest payroll</span>
                        <Activity className="h-4 w-4 text-aqua" />
                      </div>
                      <h3 className="mt-3 font-display text-2xl text-white">{insights?.topPayroll?.name || "N/A"}</h3>
                      <p className="mt-2 text-sm text-slate-400">{formatCurrency(insights?.topPayroll?.salary)} compensation level</p>
                    </div>
                  </div>
                )}
              </div>

              <div className="rounded-[2rem] border border-white/10 bg-white/[0.06] p-6 backdrop-blur-xl">
                <p className="text-xs uppercase tracking-[0.28em] text-slate-400">Directory search</p>
                <FormField
                  label="People finder"
                  className="mt-3"
                  value={query}
                  onChange={(event) => setQuery(event.target.value)}
                  placeholder="Search clients, employees, and admins by name or ID"
                />
              </div>
            </div>
          </div>
        </section>

        <div className="grid gap-6 xl:grid-cols-4">
          {summaryCards.map((card) => (
            <ManagerMetricCard key={card.label} {...card} />
          ))}
        </div>

        <div className="grid gap-6 xl:grid-cols-[1.1fr_0.9fr]">
          <GlassCard className="overflow-hidden">
            <SectionHeading
              eyebrow="Executive Insights"
              title="System heartbeat"
              description="A cleaner overview of reserves, payroll, and leadership health for daily monitoring."
            />
            {overviewState.loading ? (
              <Skeleton className="h-72" />
            ) : (
              <div className="grid gap-4 lg:grid-cols-2">
                <div className="rounded-[1.75rem] border border-white/10 bg-gradient-to-br from-white/8 to-white/[0.03] p-5">
                  <p className="text-xs uppercase tracking-[0.24em] text-slate-400">Total payroll</p>
                  <h5 className="mt-3 font-display text-3xl text-white">{formatCurrency(metrics?.totalPayroll)}</h5>
                  <p className="mt-3 text-sm leading-7 text-slate-300">Employees, admins, and the manager account are all represented in a single staffing cost view.</p>
                </div>
                <div className="rounded-[1.75rem] border border-white/10 bg-gradient-to-br from-white/8 to-white/[0.03] p-5">
                  <p className="text-xs uppercase tracking-[0.24em] text-slate-400">Reserve strength</p>
                  <h5 className="mt-3 font-display text-3xl text-white">{formatCurrency(metrics?.portfolioReserve)}</h5>
                  <p className="mt-3 text-sm leading-7 text-slate-300">This highlights the amount above mandatory protected client minimum balances.</p>
                </div>
                <div className="rounded-[1.75rem] border border-white/10 bg-gradient-to-br from-white/8 to-white/[0.03] p-5 lg:col-span-2">
                  <div className="flex items-center justify-between gap-4">
                    <div>
                      <p className="text-xs uppercase tracking-[0.24em] text-slate-400">Leadership composition</p>
                      <h5 className="mt-3 font-display text-2xl text-white">Balanced staffing view</h5>
                    </div>
                    <Building2 className="h-5 w-5 text-aqua" />
                  </div>
                  <div className="mt-5 grid gap-4 md:grid-cols-3">
                    <div className="rounded-2xl bg-white/5 p-4">
                      <p className="text-sm text-slate-400">Clients</p>
                      <p className="mt-2 text-2xl font-semibold text-white">{metrics?.clientsCount ?? 0}</p>
                    </div>
                    <div className="rounded-2xl bg-white/5 p-4">
                      <p className="text-sm text-slate-400">Employees</p>
                      <p className="mt-2 text-2xl font-semibold text-white">{metrics?.employeesCount ?? 0}</p>
                    </div>
                    <div className="rounded-2xl bg-white/5 p-4">
                      <p className="text-sm text-slate-400">Admins</p>
                      <p className="mt-2 text-2xl font-semibold text-white">{metrics?.adminsCount ?? 0}</p>
                    </div>
                  </div>
                </div>
              </div>
            )}
          </GlassCard>

          <GlassCard className="overflow-hidden">
            <SectionHeading
              eyebrow="Transaction Desk"
              title="Corrective actions"
              description="A dedicated console for manager-triggered deposits, withdrawals, and transfers."
            />
            <div className="space-y-4">
              <div className="rounded-[1.75rem] border border-cyan-300/10 bg-gradient-to-br from-cyan-300/10 to-white/[0.03] p-5">
                <div className="flex items-center justify-between gap-3">
                  <div>
                    <p className="text-xs uppercase tracking-[0.24em] text-slate-400">Single client action</p>
                    <h5 className="mt-2 font-display text-2xl text-white">Deposit or withdraw</h5>
                  </div>
                  <Wallet className="h-5 w-5 text-aqua" />
                </div>
                <div className="mt-5 grid gap-4 md:grid-cols-2">
                  <FormField label="Client ID" type="number" value={deskForm.clientId} onChange={(event) => setDeskForm((current) => ({ ...current, clientId: event.target.value }))} />
                  <FormField label="Amount" type="number" value={deskForm.amount} onChange={(event) => setDeskForm((current) => ({ ...current, amount: event.target.value }))} />
                </div>
                <div className="mt-4 grid gap-3 md:grid-cols-2">
                  <button onClick={() => runDeskAction("deposit")} disabled={deskLoading === "deposit"} className="rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
                    {deskLoading === "deposit" ? "Processing..." : "Deposit funds"}
                  </button>
                  <button onClick={() => runDeskAction("withdraw")} disabled={deskLoading === "withdraw"} className="rounded-2xl border border-white/10 bg-white/5 px-4 py-3 text-sm font-semibold text-white">
                    {deskLoading === "withdraw" ? "Processing..." : "Withdraw funds"}
                  </button>
                </div>
              </div>

              <div className="rounded-[1.75rem] border border-fuchsia-300/10 bg-gradient-to-br from-fuchsia-300/10 to-white/[0.03] p-5">
                <div className="flex items-center justify-between gap-3">
                  <div>
                    <p className="text-xs uppercase tracking-[0.24em] text-slate-400">Cross-account move</p>
                    <h5 className="mt-2 font-display text-2xl text-white">Transfer between clients</h5>
                  </div>
                  <ArrowRightLeft className="h-5 w-5 text-iris" />
                </div>
                <div className="mt-5 grid gap-4 md:grid-cols-3">
                  <FormField label="From Client ID" type="number" value={deskForm.fromClientId} onChange={(event) => setDeskForm((current) => ({ ...current, fromClientId: event.target.value }))} />
                  <FormField label="To Client ID" type="number" value={deskForm.toClientId} onChange={(event) => setDeskForm((current) => ({ ...current, toClientId: event.target.value }))} />
                  <FormField label="Amount" type="number" value={deskForm.transferAmount} onChange={(event) => setDeskForm((current) => ({ ...current, transferAmount: event.target.value }))} />
                </div>
                <button onClick={() => runDeskAction("transfer")} disabled={deskLoading === "transfer"} className="mt-4 w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">
                  {deskLoading === "transfer" ? "Processing..." : "Transfer funds"}
                </button>
              </div>
            </div>
          </GlassCard>
        </div>

        <section className="grid gap-6 2xl:grid-cols-3">
          <GlassCard>
            <SectionHeading
              eyebrow="Admin Control"
              title="Admins"
              description="The manager creates and updates admins here."
              action={
                <button onClick={() => setOpenAdminModal(true)} className="rounded-2xl border border-white/10 px-4 py-2 text-sm font-semibold text-white transition hover:bg-white/10">
                  Add admin
                </button>
              }
            />
            {adminsState.loading ? <Skeleton className="h-72" /> : <DataTable columns={adminColumns} rows={filteredAdmins} emptyMessage="No admins available." />}
          </GlassCard>

          <GlassCard>
            <SectionHeading
              eyebrow="Team Operations"
              title="Employees"
              description="Direct visibility into the operational team."
              action={
                <button onClick={() => setOpenEmployeeModal(true)} className="rounded-2xl border border-white/10 px-4 py-2 text-sm font-semibold text-white transition hover:bg-white/10">
                  Add employee
                </button>
              }
            />
            {employeesState.loading ? <Skeleton className="h-72" /> : <DataTable columns={employeeColumns} rows={filteredEmployees} emptyMessage="No employees available." />}
          </GlassCard>

          <GlassCard>
            <SectionHeading
              eyebrow="Portfolio Accounts"
              title="Clients"
              description="Client records with direct correction access."
              action={
                <button onClick={() => setOpenClientModal(true)} className="rounded-2xl border border-white/10 px-4 py-2 text-sm font-semibold text-white transition hover:bg-white/10">
                  Add client
                </button>
              }
            />
            {clientsState.loading ? <Skeleton className="h-72" /> : <DataTable columns={clientColumns} rows={filteredClients} emptyMessage="No clients available." />}
          </GlassCard>
        </section>
      </div>

      <ActionModal open={openClientModal} title={editingClient ? "Edit Client" : "Add Client"} onClose={closeClientModal}>
        <form className="space-y-4" onSubmit={submitClient}>
          <FormField label="Name" required value={clientForm.name} onChange={(event) => setClientForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={clientForm.password} onChange={(event) => setClientForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Balance" type="number" min="1500" required value={clientForm.balance} onChange={(event) => setClientForm((current) => ({ ...current, balance: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">
            {submitting ? "Saving..." : editingClient ? "Update client" : "Create client"}
          </button>
        </form>
      </ActionModal>

      <ActionModal open={openEmployeeModal} title={editingEmployee ? "Edit Employee" : "Add Employee"} onClose={closeEmployeeModal}>
        <form className="space-y-4" onSubmit={submitEmployee}>
          <FormField label="Name" required value={employeeForm.name} onChange={(event) => setEmployeeForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={employeeForm.password} onChange={(event) => setEmployeeForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Salary" type="number" min="5000" required value={employeeForm.salary} onChange={(event) => setEmployeeForm((current) => ({ ...current, salary: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
            {submitting ? "Saving..." : editingEmployee ? "Update employee" : "Create employee"}
          </button>
        </form>
      </ActionModal>

      <ActionModal open={openAdminModal} title={editingAdmin ? "Edit Admin" : "Add Admin"} onClose={closeAdminModal}>
        <form className="space-y-4" onSubmit={submitAdmin}>
          <FormField label="Name" required value={adminForm.name} onChange={(event) => setAdminForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={adminForm.password} onChange={(event) => setAdminForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Salary" type="number" min="5000" required value={adminForm.salary} onChange={(event) => setAdminForm((current) => ({ ...current, salary: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
            {submitting ? "Saving..." : editingAdmin ? "Update admin" : "Create admin"}
          </button>
        </form>
      </ActionModal>
    </AppShell>
  );
}
