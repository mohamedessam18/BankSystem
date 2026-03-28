import { useMemo, useState } from "react";
import AppShell from "../layouts/AppShell";
import ActionModal from "../components/ActionModal";
import DataTable from "../components/DataTable";
import FormField from "../components/FormField";
import GlassCard from "../components/GlassCard";
import { Skeleton } from "../components/Loader";
import { adminApi, employeeApi, extractApiError, managerApi } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";
import { useToast } from "../context/ToastContext";

const blankClient = { name: "", password: "", balance: "1500" };
const blankEmployee = { name: "", password: "", salary: "5000" };
const blankAdmin = { name: "", password: "", salary: "5000" };

export default function ManagerPeopleControlPage() {
  const { pushToast } = useToast();
  const [query, setQuery] = useState("");
  const [submitting, setSubmitting] = useState(false);
  const [clientForm, setClientForm] = useState(blankClient);
  const [employeeForm, setEmployeeForm] = useState(blankEmployee);
  const [adminForm, setAdminForm] = useState(blankAdmin);
  const [editingClient, setEditingClient] = useState(null);
  const [editingEmployee, setEditingEmployee] = useState(null);
  const [editingAdmin, setEditingAdmin] = useState(null);
  const [openClientModal, setOpenClientModal] = useState(false);
  const [openEmployeeModal, setOpenEmployeeModal] = useState(false);
  const [openAdminModal, setOpenAdminModal] = useState(false);

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

  const searchable = (row) => `${row.name} ${row.id}`.toLowerCase().includes(query.toLowerCase());
  const filteredClients = useMemo(() => (clientsState.data || []).filter(searchable), [clientsState.data, query]);
  const filteredEmployees = useMemo(() => (employeesState.data || []).filter(searchable), [employeesState.data, query]);
  const filteredAdmins = useMemo(() => (adminsState.data || []).filter(searchable), [adminsState.data, query]);

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
      setOpenClientModal(false);
      setEditingClient(null);
      setClientForm(blankClient);
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
      await refreshEmployees();
      setOpenEmployeeModal(false);
      setEditingEmployee(null);
      setEmployeeForm(blankEmployee);
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
      await refreshAdmins();
      setOpenAdminModal(false);
      setEditingAdmin(null);
      setAdminForm(blankAdmin);
      pushToast({ title: editingAdmin ? "Admin updated successfully." : "Admin created successfully." });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setSubmitting(false);
    }
  };

  const clientColumns = [
    { key: "id", label: "Client ID" },
    { key: "name", label: "Name" },
    { key: "balance", label: "Balance", render: (row) => `$${Number(row.balance).toLocaleString()}` },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button onClick={() => { setEditingClient(row); setClientForm({ name: row.name, password: "", balance: String(row.balance) }); setOpenClientModal(true); }} className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10">
          Edit
        </button>
      )
    }
  ];

  const employeeColumns = [
    { key: "id", label: "Employee ID" },
    { key: "name", label: "Name" },
    { key: "salary", label: "Salary", render: (row) => `$${Number(row.salary).toLocaleString()}` },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button onClick={() => { setEditingEmployee(row); setEmployeeForm({ name: row.name, password: "", salary: String(row.salary) }); setOpenEmployeeModal(true); }} className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10">
          Edit
        </button>
      )
    }
  ];

  const adminColumns = [
    { key: "id", label: "Admin ID" },
    { key: "name", label: "Name" },
    { key: "salary", label: "Salary", render: (row) => `$${Number(row.salary).toLocaleString()}` },
    {
      key: "action",
      label: "Action",
      render: (row) => (
        <button onClick={() => { setEditingAdmin(row); setAdminForm({ name: row.name, password: "", salary: String(row.salary) }); setOpenAdminModal(true); }} className="rounded-full border border-white/10 px-4 py-2 text-xs font-semibold uppercase tracking-[0.2em] text-slate-200 transition hover:bg-white/10">
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
              <p className="text-sm uppercase tracking-[0.35em] text-aqua">People control</p>
              <h3 className="mt-3 font-display text-3xl text-white">Manage admins, employees, and clients</h3>
            </div>
            <input
              value={query}
              onChange={(event) => setQuery(event.target.value)}
              placeholder="Search by name or ID"
              className="rounded-2xl border border-white/10 bg-white/5 px-4 py-3 text-sm text-white outline-none placeholder:text-slate-500"
            />
          </div>
        </GlassCard>

        <div className="grid gap-6 2xl:grid-cols-3">
          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Admins</h4>
              <button onClick={() => setOpenAdminModal(true)} className="rounded-2xl border border-white/10 px-4 py-2 text-sm font-semibold text-white transition hover:bg-white/10">Add</button>
            </div>
            {adminsState.loading ? <Skeleton className="h-72" /> : <DataTable columns={adminColumns} rows={filteredAdmins} emptyMessage="No admins available." />}
          </GlassCard>

          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Employees</h4>
              <button onClick={() => setOpenEmployeeModal(true)} className="rounded-2xl border border-white/10 px-4 py-2 text-sm font-semibold text-white transition hover:bg-white/10">Add</button>
            </div>
            {employeesState.loading ? <Skeleton className="h-72" /> : <DataTable columns={employeeColumns} rows={filteredEmployees} emptyMessage="No employees available." />}
          </GlassCard>

          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Clients</h4>
              <button onClick={() => setOpenClientModal(true)} className="rounded-2xl border border-white/10 px-4 py-2 text-sm font-semibold text-white transition hover:bg-white/10">Add</button>
            </div>
            {clientsState.loading ? <Skeleton className="h-72" /> : <DataTable columns={clientColumns} rows={filteredClients} emptyMessage="No clients available." />}
          </GlassCard>
        </div>
      </div>

      <ActionModal open={openClientModal} title={editingClient ? "Edit Client" : "Add Client"} onClose={() => { setOpenClientModal(false); setEditingClient(null); setClientForm(blankClient); }}>
        <form className="space-y-4" onSubmit={submitClient}>
          <FormField label="Name" required value={clientForm.name} onChange={(event) => setClientForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={clientForm.password} onChange={(event) => setClientForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Balance" type="number" min="1500" required value={clientForm.balance} onChange={(event) => setClientForm((current) => ({ ...current, balance: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">{submitting ? "Saving..." : editingClient ? "Update client" : "Create client"}</button>
        </form>
      </ActionModal>

      <ActionModal open={openEmployeeModal} title={editingEmployee ? "Edit Employee" : "Add Employee"} onClose={() => { setOpenEmployeeModal(false); setEditingEmployee(null); setEmployeeForm(blankEmployee); }}>
        <form className="space-y-4" onSubmit={submitEmployee}>
          <FormField label="Name" required value={employeeForm.name} onChange={(event) => setEmployeeForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={employeeForm.password} onChange={(event) => setEmployeeForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Salary" type="number" min="5000" required value={employeeForm.salary} onChange={(event) => setEmployeeForm((current) => ({ ...current, salary: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">{submitting ? "Saving..." : editingEmployee ? "Update employee" : "Create employee"}</button>
        </form>
      </ActionModal>

      <ActionModal open={openAdminModal} title={editingAdmin ? "Edit Admin" : "Add Admin"} onClose={() => { setOpenAdminModal(false); setEditingAdmin(null); setAdminForm(blankAdmin); }}>
        <form className="space-y-4" onSubmit={submitAdmin}>
          <FormField label="Name" required value={adminForm.name} onChange={(event) => setAdminForm((current) => ({ ...current, name: event.target.value }))} />
          <FormField label="Password" type="password" required value={adminForm.password} onChange={(event) => setAdminForm((current) => ({ ...current, password: event.target.value }))} />
          <FormField label="Salary" type="number" min="5000" required value={adminForm.salary} onChange={(event) => setAdminForm((current) => ({ ...current, salary: event.target.value }))} />
          <button disabled={submitting} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">{submitting ? "Saving..." : editingAdmin ? "Update admin" : "Create admin"}</button>
        </form>
      </ActionModal>
    </AppShell>
  );
}
