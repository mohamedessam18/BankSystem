import { useState } from "react";
import { ArrowDownLeft, ArrowLeftRight, ArrowUpRight } from "lucide-react";
import AppShell from "../layouts/AppShell";
import GlassCard from "../components/GlassCard";
import FormField from "../components/FormField";
import { Skeleton } from "../components/Loader";
import { useAuth } from "../context/AuthContext";
import { useToast } from "../context/ToastContext";
import { clientApi, extractApiError } from "../services/api";
import { useDashboardData } from "../hooks/useDashboardData";

export default function ClientDashboardPage() {
  const { session } = useAuth();
  const { pushToast } = useToast();
  const [loadingAction, setLoadingAction] = useState("");
  const [forms, setForms] = useState({
    deposit: "",
    withdraw: "",
    transferAmount: "",
    transferTarget: ""
  });

  const { data, setData, loading } = useDashboardData(async () => {
    const response = await clientApi.getBalance(session.user.id);
    return response.data.data;
  }, [session.user.id]);

  const runAction = async (type) => {
    setLoadingAction(type);
    try {
      let response;
      if (type === "deposit") {
        response = await clientApi.deposit(session.user.id, Number(forms.deposit));
      } else if (type === "withdraw") {
        response = await clientApi.withdraw(session.user.id, Number(forms.withdraw));
      } else {
        response = await clientApi.transfer({
          fromClientId: session.user.id,
          toClientId: Number(forms.transferTarget),
          amount: Number(forms.transferAmount)
        });
      }

      const balanceResponse = await clientApi.getBalance(session.user.id);
      setData(balanceResponse.data.data);
      pushToast({ title: response.data.message });
      setForms({ deposit: "", withdraw: "", transferAmount: "", transferTarget: "" });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setLoadingAction("");
    }
  };

  const history = data?.analytics?.history ?? [];

  return (
    <AppShell>
      <div className="grid gap-6">
        {loading ? (
          <>
            <Skeleton className="h-48" />
            <Skeleton className="h-64" />
          </>
        ) : (
          <>
            <GlassCard className="overflow-hidden">
              <div className="relative">
                <div className="absolute -right-12 -top-14 h-40 w-40 rounded-full bg-aqua/20 blur-3xl" />
                <p className="text-sm uppercase tracking-[0.4em] text-aqua">Available balance</p>
                <h3 className="mt-6 font-display text-5xl">${data.client.balance.toLocaleString()}</h3>
                <p className="mt-3 max-w-xl text-sm leading-7 text-slate-300">
                  Real-time file-backed balance synced through the C++ API layer with minimum-balance protection.
                </p>
              </div>
            </GlassCard>

            <div className="grid gap-6 xl:grid-cols-[1.1fr_0.9fr]">
              <GlassCard>
                <div className="mb-6 flex items-center justify-between">
                  <h3 className="font-display text-2xl text-white">Quick actions</h3>
                  <span className="text-xs uppercase tracking-[0.3em] text-slate-400">Transactions</span>
                </div>
                <div className="grid gap-5 md:grid-cols-2">
                  <div className="space-y-4 rounded-3xl border border-white/10 bg-white/5 p-5">
                    <div className="flex items-center gap-3">
                      <ArrowDownLeft className="h-5 w-5 text-aqua" />
                      <p className="font-semibold">Deposit</p>
                    </div>
                    <FormField label="Amount" type="number" value={forms.deposit} onChange={(event) => setForms((current) => ({ ...current, deposit: event.target.value }))} />
                    <button onClick={() => runAction("deposit")} disabled={loadingAction === "deposit"} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink transition hover:shadow-glow disabled:opacity-60">
                      {loadingAction === "deposit" ? "Processing..." : "Deposit funds"}
                    </button>
                  </div>

                  <div className="space-y-4 rounded-3xl border border-white/10 bg-white/5 p-5">
                    <div className="flex items-center gap-3">
                      <ArrowUpRight className="h-5 w-5 text-sunset" />
                      <p className="font-semibold">Withdraw</p>
                    </div>
                    <FormField label="Amount" type="number" value={forms.withdraw} onChange={(event) => setForms((current) => ({ ...current, withdraw: event.target.value }))} />
                    <button onClick={() => runAction("withdraw")} disabled={loadingAction === "withdraw"} className="w-full rounded-2xl bg-white/10 px-4 py-3 text-sm font-semibold text-white transition hover:bg-white/[0.15] disabled:opacity-60">
                      {loadingAction === "withdraw" ? "Processing..." : "Withdraw funds"}
                    </button>
                  </div>

                  <div className="space-y-4 rounded-3xl border border-white/10 bg-white/5 p-5 md:col-span-2">
                    <div className="flex items-center gap-3">
                      <ArrowLeftRight className="h-5 w-5 text-iris" />
                      <p className="font-semibold">Transfer</p>
                    </div>
                    <div className="grid gap-4 md:grid-cols-2">
                      <FormField label="Recipient Client ID" type="number" value={forms.transferTarget} onChange={(event) => setForms((current) => ({ ...current, transferTarget: event.target.value }))} />
                      <FormField label="Amount" type="number" value={forms.transferAmount} onChange={(event) => setForms((current) => ({ ...current, transferAmount: event.target.value }))} />
                    </div>
                    <button onClick={() => runAction("transfer")} disabled={loadingAction === "transfer"} className="w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white transition hover:shadow-glow disabled:opacity-60">
                      {loadingAction === "transfer" ? "Processing..." : "Transfer funds"}
                    </button>
                  </div>
                </div>
              </GlassCard>

              <GlassCard>
                <div className="mb-6 flex items-center justify-between">
                  <h3 className="font-display text-2xl text-white">Balance outlook</h3>
                  <span className="text-xs uppercase tracking-[0.3em] text-slate-400">Snapshot</span>
                </div>
                <div className="space-y-4">
                  {history.map((point) => (
                    <div key={point.label}>
                      <div className="mb-2 flex items-center justify-between text-sm text-slate-300">
                        <span>{point.label}</span>
                        <span>${Number(point.value).toLocaleString()}</span>
                      </div>
                      <div className="h-3 rounded-full bg-white/[0.06]">
                        <div
                          className="h-3 rounded-full bg-gradient-to-r from-aqua via-iris to-sunset"
                          style={{ width: `${Math.min((Number(point.value) / Math.max(data.client.balance * 1.1, 1)) * 100, 100)}%` }}
                        />
                      </div>
                    </div>
                  ))}
                </div>
              </GlassCard>
            </div>
          </>
        )}
      </div>
    </AppShell>
  );
}
