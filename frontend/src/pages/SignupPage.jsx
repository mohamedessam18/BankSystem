import { useState } from "react";
import { motion } from "framer-motion";
import { Link, useNavigate } from "react-router-dom";
import FormField from "../components/FormField";
import { Spinner } from "../components/Loader";
import { useAuth } from "../context/AuthContext";
import { useToast } from "../context/ToastContext";

export default function SignupPage() {
  const [form, setForm] = useState({ role: "client", name: "", password: "", initialBalance: "1500", salary: "5000" });
  const { signup, loading } = useAuth();
  const { pushToast } = useToast();
  const navigate = useNavigate();

  const submit = async (event) => {
    event.preventDefault();
    try {
      const payload = {
        role: form.role,
        name: form.name,
        password: form.password
      };

      if (form.role === "client") {
        payload.initialBalance = Number(form.initialBalance);
      } else {
        payload.salary = Number(form.salary);
      }

      const session = await signup(payload);
      pushToast({ title: `Welcome ${session.user.name}. Your ID is ${session.user.id}.` });
      navigate(`/dashboard/${session.role}`);
    } catch (error) {
      pushToast({ title: error.message, type: "error" });
    }
  };

  const amountLabel = form.role === "client" ? "Initial Balance" : "Salary";
  const amountValue = form.role === "client" ? form.initialBalance : form.salary;
  const amountMin = form.role === "client" ? "1500" : "5000";

  return (
    <div className="min-h-screen bg-ink bg-mesh px-4 py-10 text-white">
      <motion.div initial={{ opacity: 0, y: 24 }} animate={{ opacity: 1, y: 0 }} className="mx-auto max-w-md rounded-[2rem] border border-white/10 bg-white/[0.06] p-8 shadow-panel backdrop-blur-2xl">
        <p className="text-sm uppercase tracking-[0.4em] text-aqua">Role onboarding</p>
        <h1 className="mt-4 font-display text-4xl">Sign Up</h1>
        <p className="mt-3 text-sm leading-7 text-slate-300">Create a secure client, employee, or admin account. The backend assigns your ID automatically.</p>
        <form className="mt-8 space-y-5" onSubmit={submit}>
          <label className="block space-y-2">
            <span className="text-sm font-medium text-slate-300">Role</span>
            <select
              value={form.role}
              onChange={(event) => setForm((current) => ({ ...current, role: event.target.value }))}
              className="w-full rounded-2xl border border-white/10 bg-white/5 px-4 py-3 text-sm text-white outline-none transition focus:border-aqua/60 focus:bg-white/10"
            >
              <option value="client" className="bg-shell">Client</option>
              <option value="employee" className="bg-shell">Employee</option>
              <option value="admin" className="bg-shell">Admin</option>
            </select>
          </label>
          <FormField label="Name" required value={form.name} onChange={(event) => setForm((current) => ({ ...current, name: event.target.value }))} placeholder="Full name" />
          <FormField label="Password" type="password" required value={form.password} onChange={(event) => setForm((current) => ({ ...current, password: event.target.value }))} placeholder="8-20 characters, no spaces" />
          <FormField
            label={amountLabel}
            type="number"
            min={amountMin}
            required
            value={amountValue}
            onChange={(event) =>
              setForm((current) =>
                form.role === "client"
                  ? { ...current, initialBalance: event.target.value }
                  : { ...current, salary: event.target.value }
              )
            }
          />
          <button disabled={loading} className="flex w-full items-center justify-center gap-3 rounded-2xl bg-aqua px-4 py-3 font-semibold text-ink transition hover:shadow-glow disabled:cursor-not-allowed disabled:opacity-70">
            {loading ? <Spinner /> : `Create ${form.role} account`}
          </button>
        </form>
        <p className="mt-6 text-sm text-slate-400">
          Already have an account?{" "}
          <Link className="text-aqua" to="/login">
            Login
          </Link>
        </p>
      </motion.div>
    </div>
  );
}
