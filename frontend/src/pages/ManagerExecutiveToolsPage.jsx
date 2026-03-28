import { useState } from "react";
import { ArrowRightLeft, Landmark, ShieldCheck, Wallet } from "lucide-react";
import AppShell from "../layouts/AppShell";
import GlassCard from "../components/GlassCard";
import FormField from "../components/FormField";
import { Skeleton } from "../components/Loader";
import { clientApi, extractApiError, managerApi } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";
import { useToast } from "../context/ToastContext";

const blankDesk = { clientId: "", amount: "", fromClientId: "", toClientId: "", transferAmount: "" };

export default function ManagerExecutiveToolsPage() {
  const { pushToast } = useToast();
  const [deskForm, setDeskForm] = useState(blankDesk);
  const [deskLoading, setDeskLoading] = useState("");

  const overviewState = useDashboardData(async () => {
    const response = await managerApi.getOverview();
    return response.data.data;
  }, []);

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
      pushToast({ title: `Manager ${type} request completed successfully.` });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setDeskLoading("");
    }
  };

  const metrics = overviewState.data?.metrics;

  return (
    <AppShell>
      <div className="grid gap-6">
        <GlassCard>
          <p className="text-sm uppercase tracking-[0.35em] text-aqua">Executive tools</p>
          <h3 className="mt-3 font-display text-3xl text-white">Manager action center</h3>
          <p className="mt-3 max-w-2xl text-sm leading-7 text-slate-300">
            Use these tools for recovery actions, reserve monitoring, and operational intervention across client accounts.
          </p>
        </GlassCard>

        <div className="grid gap-6 xl:grid-cols-[1.05fr_0.95fr]">
          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Transaction desk</h4>
              <ArrowRightLeft className="h-5 w-5 text-aqua" />
            </div>
            <div className="space-y-4">
              <div className="rounded-3xl border border-white/10 bg-white/5 p-5">
                <div className="grid gap-4 md:grid-cols-2">
                  <FormField label="Client ID" type="number" value={deskForm.clientId} onChange={(event) => setDeskForm((current) => ({ ...current, clientId: event.target.value }))} />
                  <FormField label="Amount" type="number" value={deskForm.amount} onChange={(event) => setDeskForm((current) => ({ ...current, amount: event.target.value }))} />
                </div>
                <div className="mt-4 grid gap-3 md:grid-cols-2">
                  <button onClick={() => runDeskAction("deposit")} disabled={deskLoading === "deposit"} className="rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
                    {deskLoading === "deposit" ? "Processing..." : "Deposit for client"}
                  </button>
                  <button onClick={() => runDeskAction("withdraw")} disabled={deskLoading === "withdraw"} className="rounded-2xl bg-white/10 px-4 py-3 text-sm font-semibold text-white">
                    {deskLoading === "withdraw" ? "Processing..." : "Withdraw for client"}
                  </button>
                </div>
              </div>

              <div className="rounded-3xl border border-white/10 bg-white/5 p-5">
                <div className="grid gap-4 md:grid-cols-3">
                  <FormField label="From Client ID" type="number" value={deskForm.fromClientId} onChange={(event) => setDeskForm((current) => ({ ...current, fromClientId: event.target.value }))} />
                  <FormField label="To Client ID" type="number" value={deskForm.toClientId} onChange={(event) => setDeskForm((current) => ({ ...current, toClientId: event.target.value }))} />
                  <FormField label="Amount" type="number" value={deskForm.transferAmount} onChange={(event) => setDeskForm((current) => ({ ...current, transferAmount: event.target.value }))} />
                </div>
                <button onClick={() => runDeskAction("transfer")} disabled={deskLoading === "transfer"} className="mt-4 w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">
                  {deskLoading === "transfer" ? "Processing..." : "Transfer between clients"}
                </button>
              </div>
            </div>
          </GlassCard>

          <GlassCard>
            <div className="mb-5 flex items-center justify-between">
              <h4 className="font-display text-2xl text-white">Oversight snapshot</h4>
              <ShieldCheck className="h-5 w-5 text-aqua" />
            </div>
            {overviewState.loading ? (
              <Skeleton className="h-72" />
            ) : (
              <div className="space-y-4">
                <div className="rounded-3xl border border-white/10 bg-white/5 p-5">
                  <div className="flex items-center gap-3">
                    <Wallet className="h-5 w-5 text-aqua" />
                    <div>
                      <p className="text-sm text-slate-400">Portfolio reserve</p>
                      <h5 className="mt-1 text-2xl font-semibold text-white">${Number(metrics?.portfolioReserve || 0).toLocaleString()}</h5>
                    </div>
                  </div>
                </div>
                <div className="rounded-3xl border border-white/10 bg-white/5 p-5">
                  <div className="flex items-center gap-3">
                    <Landmark className="h-5 w-5 text-aqua" />
                    <div>
                      <p className="text-sm text-slate-400">Total payroll</p>
                      <h5 className="mt-1 text-2xl font-semibold text-white">${Number(metrics?.totalPayroll || 0).toLocaleString()}</h5>
                    </div>
                  </div>
                </div>
                <div className="rounded-3xl border border-white/10 bg-white/5 p-5">
                  <p className="text-xs uppercase tracking-[0.25em] text-slate-400">Manager note</p>
                  <p className="mt-3 text-sm leading-7 text-slate-300">
                    These tools are intentionally separate from people management so the manager can execute urgent operational actions without leaving the executive workspace.
                  </p>
                </div>
              </div>
            )}
          </GlassCard>
        </div>
      </div>
    </AppShell>
  );
}
