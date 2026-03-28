import { createContext, useContext, useEffect, useMemo, useState } from "react";
import { notificationApi } from "../services/api";
import { useAuth } from "./AuthContext";

const NotificationContext = createContext(null);

export function NotificationProvider({ children }) {
  const { session } = useAuth();
  const [items, setItems] = useState([]);
  const [unreadCount, setUnreadCount] = useState(0);
  const [loading, setLoading] = useState(false);

  const refresh = async () => {
    if (!session?.role) {
      setItems([]);
      setUnreadCount(0);
      return;
    }

    setLoading(true);
    try {
      const response = await notificationApi.getNotifications(session.role, session.user?.id);
      setItems(response.data.data.notifications || []);
      setUnreadCount(response.data.data.unreadCount || 0);
    } finally {
      setLoading(false);
    }
  };

  const markAllRead = async () => {
    if (!session?.role) {
      return;
    }

    await notificationApi.markRead(session.role, session.user?.id);
    setItems((current) => current.map((item) => ({ ...item, read: true })));
    setUnreadCount(0);
  };

  useEffect(() => {
    refresh();

    if (!session?.role) {
      return undefined;
    }

    const intervalId = window.setInterval(refresh, 7000);
    return () => window.clearInterval(intervalId);
  }, [session?.role]);

  const value = useMemo(
    () => ({
      items,
      unreadCount,
      loading,
      refresh,
      markAllRead
    }),
    [items, unreadCount, loading]
  );

  return <NotificationContext.Provider value={value}>{children}</NotificationContext.Provider>;
}

export function useNotifications() {
  return useContext(NotificationContext);
}
