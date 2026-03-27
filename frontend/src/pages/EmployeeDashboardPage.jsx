import { useMemo, useState } from "react";
import AppShell from "../layouts/AppShell";
import ActionModal from "../components/ActionModal";
import DataTable from "../components/DataTable";
import FormField from "../components/FormField";
import GlassCard from "../components/GlassCard";
import { Skeleton } from "../components/Loader";
import { employeeApi, extractApiError } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";
import { useToast } from "../context/ToastContext";

const blankClient = { name: "", password: "", balance: "1500" };

export default function EmployeeDashboardPage() {
  const { pushToast } = useToast();
  const [query, setQuery] = useState("");
  const [openAdd, setOpenAdd] = useState(false);
  const [editing, setEditing] = useState(null);
  const [form, setForm] = useState(blankClient);
  const [submitting, setSubmitting] = useState(false);

  const { data, setData, loading } = useDashboardData(async () => {
    const response = await employeeApi.getClients();
    return response.data.data.clients;
  }, []);

  const rows = useMemo(
    () => (data || []).filter((client) => `${client.name} ${client.id}`.toLowerCase().includes(query.toLowerCase())),
    [data, query]
  );

  const resetForm = () => setForm(blankClient);

  const refreshClients = async () => {
    const response = await employeeApi.getClients();
    setData(response.data.data.clients);
  };

  const submitNewClient = async (event) => {
    event.preventDefault();
    setSubmitting(true);
    try {
      await employeeApi.addClient({ ...form, balance: Number(form.balance) });
      await refreshClients();
      setOpenAdd(false);
      resetForm();
      pushToast({ title: "Client created successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const submitEditClient = async (event) => {
    event.preventDefault();
    setSubmitting(true);
    try {
      await employeeApi.updateClient(editing.id, { ...form, balance: Number(form.balance) });
      await refreshClients();
      setEditing(null);
      resetForm();
      pushToast({ title: "Client updated successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const columns = [
    { key: "id", label: "ID" },
    { key: "name", label: "Client Name" },
    {
      key: "balance",
      label: "Balance",
      render: (row) => `$${Number(row.balance).toLocaleString()}`
    },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button
          onClick={() => {
            setEditing(row);
            setForm({ name: row.name, password: "", balance: String(row.balance) });
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
        <GlassCard>
          <div className="flex flex-col gap-4 md:flex-row md:items-center md:justify-between">
            <div>
              <p className="text-sm uppercase tracking-[0.35em] text-aqua">Client operations</p>
              <h3 className="mt-3 font-display text-3xl text-white">Employee dashboard</h3>
            </div>
            <div className="flex flex-col gap-3 md:flex-row">
              <input
                value={query}
                onChange={(event) => setQuery(event.target.value)}
                placeholder="Search by name or ID"
                className="rounded-2xl border border-white/10 bg-white/5 px-4 py-3 text-sm text-white outline-none placeholder:text-slate-500"
              />
              <button onClick={() => setOpenAdd(true)} className="rounded-2xl bg-aqua px-5 py-3 text-sm font-semibold text-ink transition hover:shadow-glow">
                Add client
              </button>
            </div>
          </div>
        </GlassCard>

        <GlassCard>
          {loading ? <Skeleton className="h-72" /> : <DataTable columns={columns} rows={rows} emptyMessage="No clients available." />}
        </GlassCard>
      </div>

      <ActionModal open={openAdd} title="Add Client" onClose={() => { setOpenAdd(false); resetForm(); }}>
        <form className="space-y-4" onSubmit={submitNewClient}>
          <FormField label="Name" required value={form.name} onChange={(event) => setForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={form.password} onChange={(event) => setForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Initial Balance" type="number" min="1500" required value={form.balance} onChange={(event) => setForm((current) => ({ ...current, balance: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
            {submitting ? "Saving..." : "Create client"}
          </button>
        </form>
      </ActionModal>

      <ActionModal open={Boolean(editing)} title="Edit Client" onClose={() => { setEditing(null); resetForm(); }}>
        <form className="space-y-4" onSubmit={submitEditClient}>
          <FormField label="Name" required value={form.name} onChange={(event) => setForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={form.password} onChange={(event) => setForm((current) => ({ ...current, password: event.target.value }))} placeholder="Enter a new password" />
          <FormField label="Balance" type="number" min="1500" required value={form.balance} onChange={(event) => setForm((current) => ({ ...current, balance: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">
            {submitting ? "Updating..." : "Update client"}
          </button>
        </form>
      </ActionModal>
    </AppShell>
  );
}
