import { useMemo, useState } from "react";
import AppShell from "../layouts/AppShell";
import ActionModal from "../components/ActionModal";
import DataTable from "../components/DataTable";
import FormField from "../components/FormField";
import GlassCard from "../components/GlassCard";
import { Skeleton } from "../components/Loader";
import { adminApi, employeeApi, extractApiError } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";
import { useToast } from "../context/ToastContext";

const blankEmployee = { name: "", password: "", salary: "5000" };
const blankClient = { name: "", password: "", balance: "1500" };

export default function AdminDashboardPage() {
  const { pushToast } = useToast();
  const [query, setQuery] = useState("");
  const [openAddEmployee, setOpenAddEmployee] = useState(false);
  const [editingEmployee, setEditingEmployee] = useState(null);
  const [employeeForm, setEmployeeForm] = useState(blankEmployee);
  const [openAddClient, setOpenAddClient] = useState(false);
  const [editingClient, setEditingClient] = useState(null);
  const [clientForm, setClientForm] = useState(blankClient);
  const [submitting, setSubmitting] = useState(false);

  const clientsState = useDashboardData(async () => {
    const response = await employeeApi.getClients();
    return response.data.data.clients;
  }, []);

  const employeesState = useDashboardData(async () => {
    const response = await adminApi.getEmployees();
    return response.data.data.employees;
  }, []);

  const filteredClients = useMemo(
    () => (clientsState.data || []).filter((client) => `${client.name} ${client.id}`.toLowerCase().includes(query.toLowerCase())),
    [clientsState.data, query]
  );

  const filteredEmployees = useMemo(
    () => (employeesState.data || []).filter((employee) => `${employee.name} ${employee.id}`.toLowerCase().includes(query.toLowerCase())),
    [employeesState.data, query]
  );

  const refreshEmployees = async () => {
    const response = await adminApi.getEmployees();
    employeesState.setData(response.data.data.employees);
  };

  const refreshClients = async () => {
    const response = await employeeApi.getClients();
    clientsState.setData(response.data.data.clients);
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
      await refreshEmployees();
      setOpenAddEmployee(false);
      setEditingEmployee(null);
      setEmployeeForm(blankEmployee);
      pushToast({ title: editingEmployee ? "Employee updated successfully." : "Employee added successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
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
      await refreshClients();
      setOpenAddClient(false);
      setEditingClient(null);
      setClientForm(blankClient);
      pushToast({ title: editingClient ? "Client updated successfully." : "Client added successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const employeeColumns = [
    { key: "id", label: "ID" },
    { key: "name", label: "Employee Name" },
    {
      key: "salary",
      label: "Salary",
      render: (row) => `$${Number(row.salary).toLocaleString()}`
    },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button
          onClick={() => {
            setEditingEmployee(row);
            setEmployeeForm({ name: row.name, password: "", salary: String(row.salary) });
            setOpenAddEmployee(true);
          }}
          className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10"
        >
          Edit
        </button>
      )
    }
  ];

  const clientColumns = [
    { key: "id", label: "Client ID" },
    { key: "name", label: "Name" },
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
            setEditingClient(row);
            setClientForm({ name: row.name, password: "", balance: String(row.balance) });
            setOpenAddClient(true);
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
        {(clientsState.error || employeesState.error) && (
          <GlassCard className="border-red-400/20 bg-red-500/10">
            <p className="text-sm font-semibold text-red-100">Unable to load admin tables from the API.</p>
            <p className="mt-2 text-sm text-red-100/80">
              {employeesState.error || clientsState.error}
            </p>
          </GlassCard>
        )}

        <GlassCard>
          <div className="flex flex-col gap-4 md:flex-row md:items-center md:justify-between">
            <div>
              <p className="text-sm uppercase tracking-[0.35em] text-aqua">Administrative control</p>
              <h3 className="mt-3 font-display text-3xl text-white">Admin command center</h3>
            </div>
            <div className="flex flex-col gap-3 md:flex-row">
              <input
                value={query}
                onChange={(event) => setQuery(event.target.value)}
                placeholder="Filter people by name or ID"
                className="rounded-2xl border border-white/10 bg-white/5 px-4 py-3 text-sm text-white outline-none placeholder:text-slate-500"
              />
              <button onClick={() => setOpenAddClient(true)} className="rounded-2xl border border-white/10 px-5 py-3 text-sm font-semibold text-white transition hover:bg-white/10">
                Add client
              </button>
              <button onClick={() => setOpenAddEmployee(true)} className="rounded-2xl bg-aqua px-5 py-3 text-sm font-semibold text-ink transition hover:shadow-glow">
                Add employee
              </button>
            </div>
          </div>
        </GlassCard>

        <div className="grid gap-6 xl:grid-cols-2">
          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Employees</h4>
              <span className="text-xs uppercase tracking-[0.3em] text-slate-400">Admin only</span>
            </div>
            {employeesState.loading ? <Skeleton className="h-72" /> : <DataTable columns={employeeColumns} rows={filteredEmployees} emptyMessage="No employees available." />}
          </GlassCard>

          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Clients</h4>
              <span className="text-xs uppercase tracking-[0.3em] text-slate-400">Read view</span>
            </div>
            {clientsState.loading ? <Skeleton className="h-72" /> : <DataTable columns={clientColumns} rows={filteredClients} emptyMessage="No clients available." />}
          </GlassCard>
        </div>
      </div>

      <ActionModal
        open={openAddClient}
        title={editingClient ? "Edit Client" : "Add Client"}
        onClose={() => {
          setOpenAddClient(false);
          setEditingClient(null);
          setClientForm(blankClient);
        }}
      >
        <form className="space-y-4" onSubmit={submitClient}>
          <FormField label="Name" required value={clientForm.name} onChange={(event) => setClientForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={clientForm.password} onChange={(event) => setClientForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Balance" type="number" min="1500" required value={clientForm.balance} onChange={(event) => setClientForm((current) => ({ ...current, balance: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">
            {submitting ? "Saving..." : editingClient ? "Update client" : "Create client"}
          </button>
        </form>
      </ActionModal>

      <ActionModal
        open={openAddEmployee}
        title={editingEmployee ? "Edit Employee" : "Add Employee"}
        onClose={() => {
          setOpenAddEmployee(false);
          setEditingEmployee(null);
          setEmployeeForm(blankEmployee);
        }}
      >
        <form className="space-y-4" onSubmit={submitEmployee}>
          <FormField label="Name" required value={employeeForm.name} onChange={(event) => setEmployeeForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={employeeForm.password} onChange={(event) => setEmployeeForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Salary" type="number" min="5000" required value={employeeForm.salary} onChange={(event) => setEmployeeForm((current) => ({ ...current, salary: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
            {submitting ? "Saving..." : editingEmployee ? "Update employee" : "Create employee"}
          </button>
        </form>
      </ActionModal>
    </AppShell>
  );
}
