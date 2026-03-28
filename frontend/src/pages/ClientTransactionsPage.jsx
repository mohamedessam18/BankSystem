import { useState } from "react";
import { ArrowDownLeft, ArrowLeftRight, ArrowUpRight } from "lucide-react";
import AppShell from "../layouts/AppShell";
import GlassCard from "../components/GlassCard";
import FormField from "../components/FormField";
import { useAuth } from "../context/AuthContext";
import { useToast } from "../context/ToastContext";
import { clientApi, extractApiError } from "../services/api";

export default function ClientTransactionsPage() {
  const { session } = useAuth();
  const { pushToast } = useToast();
  const [loadingAction, setLoadingAction] = useState("");
  const [forms, setForms] = useState({
    deposit: "",
    withdraw: "",
    transferAmount: "",
    transferTarget: ""
  });

  const runAction = async (type) => {
    setLoadingAction(type);
    try {
      if (type === "deposit") {
        await clientApi.deposit(session.user.id, Number(forms.deposit));
      } else if (type === "withdraw") {
        await clientApi.withdraw(session.user.id, Number(forms.withdraw));
      } else {
        await clientApi.transfer({
          fromClientId: session.user.id,
          toClientId: Number(forms.transferTarget),
          amount: Number(forms.transferAmount)
        });
      }

      pushToast({ title: `${type[0].toUpperCase()}${type.slice(1)} completed successfully.` });
      setForms({ deposit: "", withdraw: "", transferAmount: "", transferTarget: "" });
    } catch (error) {
      pushToast({ title: extractApiError(error), type: "error" });
    } finally {
      setLoadingAction("");
    }
  };

  return (
    <AppShell>
      <div className="grid gap-6">
        <GlassCard>
          <p className="text-sm uppercase tracking-[0.35em] text-aqua">Transactions</p>
          <h3 className="mt-3 font-display text-3xl text-white">Move funds with confidence</h3>
          <p className="mt-3 max-w-2xl text-sm leading-7 text-slate-300">
            Deposit, withdraw, or transfer money from one dedicated workspace.
          </p>
        </GlassCard>

        <div className="grid gap-5 md:grid-cols-2">
          <GlassCard className="space-y-4">
            <div className="flex items-center gap-3">
              <ArrowDownLeft className="h-5 w-5 text-aqua" />
              <p className="font-semibold text-white">Deposit</p>
            </div>
            <FormField label="Amount" type="number" value={forms.deposit} onChange={(event) => setForms((current) => ({ ...current, deposit: event.target.value }))} />
            <button onClick={() => runAction("deposit")} disabled={loadingAction === "deposit"} className="w-full rounded-2xl bg-aqua px-4 py-3 text-sm font-semibold text-ink">
              {loadingAction === "deposit" ? "Processing..." : "Deposit funds"}
            </button>
          </GlassCard>

          <GlassCard className="space-y-4">
            <div className="flex items-center gap-3">
              <ArrowUpRight className="h-5 w-5 text-sunset" />
              <p className="font-semibold text-white">Withdraw</p>
            </div>
            <FormField label="Amount" type="number" value={forms.withdraw} onChange={(event) => setForms((current) => ({ ...current, withdraw: event.target.value }))} />
            <button onClick={() => runAction("withdraw")} disabled={loadingAction === "withdraw"} className="w-full rounded-2xl bg-white/10 px-4 py-3 text-sm font-semibold text-white">
              {loadingAction === "withdraw" ? "Processing..." : "Withdraw funds"}
            </button>
          </GlassCard>

          <GlassCard className="space-y-4 md:col-span-2">
            <div className="flex items-center gap-3">
              <ArrowLeftRight className="h-5 w-5 text-iris" />
              <p className="font-semibold text-white">Transfer</p>
            </div>
            <div className="grid gap-4 md:grid-cols-2">
              <FormField label="Recipient Client ID" type="number" value={forms.transferTarget} onChange={(event) => setForms((current) => ({ ...current, transferTarget: event.target.value }))} />
              <FormField label="Amount" type="number" value={forms.transferAmount} onChange={(event) => setForms((current) => ({ ...current, transferAmount: event.target.value }))} />
            </div>
            <button onClick={() => runAction("transfer")} disabled={loadingAction === "transfer"} className="w-full rounded-2xl bg-iris px-4 py-3 text-sm font-semibold text-white">
              {loadingAction === "transfer" ? "Processing..." : "Transfer funds"}
            </button>
          </GlassCard>
        </div>
      </div>
    </AppShell>
  );
}
