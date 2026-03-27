import { createContext, useContext, useEffect, useMemo, useState } from "react";
import { authApi, extractApiError } from "../services/api";

const AuthContext = createContext(null);
const STORAGE_KEY = "bank-system-session";

export function AuthProvider({ children }) {
  const [session, setSession] = useState(() => {
    const saved = window.sessionStorage.getItem(STORAGE_KEY);
    return saved ? JSON.parse(saved) : null;
  });
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    if (session) {
      window.sessionStorage.setItem(STORAGE_KEY, JSON.stringify(session));
    } else {
      window.sessionStorage.removeItem(STORAGE_KEY);
    }
  }, [session]);

  const login = async (credentials) => {
    setLoading(true);
    try {
      const response = await authApi.login(credentials);
      const nextSession = response.data.data;
      setSession(nextSession);
      return nextSession;
    } catch (error) {
      throw new Error(extractApiError(error));
    } finally {
      setLoading(false);
    }
  };

  const signup = async (payload) => {
    setLoading(true);
    try {
      const response = await authApi.signup(payload);
      const nextSession = response.data.data;
      setSession(nextSession);
      return nextSession;
    } catch (error) {
      throw new Error(extractApiError(error));
    } finally {
      setLoading(false);
    }
  };

  const logout = () => setSession(null);

  const value = useMemo(
    () => ({
      session,
      loading,
      login,
      signup,
      logout
    }),
    [session, loading]
  );

  return <AuthContext.Provider value={value}>{children}</AuthContext.Provider>;
}

export function useAuth() {
  return useContext(AuthContext);
}
